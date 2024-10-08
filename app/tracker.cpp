// Copyright 2022 Leonhard S.

#include "tracker.hpp"

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "arx.hpp"
#include "arx/ess.hpp"

#include "appdata/service-id.hpp"
#include "ess-client.hpp"
#include "game/character-info.hpp"
#include "game/state.hpp"
#include "utils.hpp"

namespace {

template <typename T>
T integerFromApiString(const arx::json_string_t& value) {
    return static_cast<T>(std::strtoull(value.c_str(), nullptr, 10));
}

} // namespace

namespace PresenceApp {

ActivityTracker::ActivityTracker(
    const CharacterData& character,
    QObject* parent
)
    : QObject(parent)
    , character_{ character }
    , state_factory_{ character.id_, character.faction_, character.server_, character.class_ }
    , current_state_{}
    , ess_client_{}
{
    // Set initial state via state factory
    state_factory_.buildState(&current_state_);
    // Create WebSocket client for event streaming endpoint
    ess_client_.reset(new EssClient(SERVICE_ID, this));
    auto subs = generateSubscriptions();
    std::for_each(subs.begin(), subs.end(),
        [this](const arx::Subscription& sub) { ess_client_->subscribe(sub); });
    ess_client_->connect();
    QObject::connect(ess_client_.get(), &EssClient::payloadReceived,
        this, &ActivityTracker::onPayloadReceived);
}

CharacterData ActivityTracker::getCharacter() const {
    return character_;
}

void ActivityTracker::onPayloadReceived(const QString& event_name,
    const arx::json_t& payload) {
    emit payloadReceived(event_name, payload);
    // Update state factory based on payload
    if (event_name == "Death") {
        handleDeathPayload(payload);
    }
    else if (event_name == "GainExperience") {
        handleGainexperiencePayload(payload);
    }
    else {
        qDebug() << "Ignoring payload for unhandled event:" << event_name;
        return;
    }
    // Check if the new state is different from the old state
    GameState state;
    if (state_factory_.buildState(&state)) {
        qWarning() << "Unable to build state from factory";
        return;
    }
    if (state != current_state_) {
        current_state_ = state;
        emit stateChanged(state);
    }
}

void ActivityTracker::handleDeathPayload(const arx::json_t& payload) {
    bool are_we_the_baddies = integerFromApiString<arx::character_id_t>(payload["attacker_character_id"]) == character_.id_;
    // Team
    ps2::Faction team = state_factory_.getFaction();
    // As Team ID is a new addition, we'll check it still exists to be safe
    if (payload.contains("team_id") && payload.contains("attacker_team_id")) {
        arx::faction_id_t team_id = integerFromApiString<arx::faction_id_t>(
            payload[are_we_the_baddies ? "attacker_team_id" : "team_id"]);
        // Return status not checked as it failing is safe as the "team"
        // variable will not be updated
        ps2::faction_from_faction_id(team_id, &team);
    }
    // Class
    arx::loadout_id_t loadout_id = integerFromApiString<arx::loadout_id_t>(
        payload[are_we_the_baddies ? "attacker_loadout_id" : "character_loadout_id"]);
    ps2::Class class_ = state_factory_.getProfileAsClass();
    if (ps2::class_from_loadout_id(loadout_id, &class_)) {
        qWarning() << "Unable to get class from loadout ID:" << loadout_id;
        return; // Do not update state if we cannot tell what class we are
    }
    // Vehicle

    // NOTE: The "vehicle_id" is not sent anymore as of March 2022. This
    // code will use it if it returns, but it is treated as optional.
    arx::vehicle_id_t vehicle_id = 0;
    if (are_we_the_baddies) {
        vehicle_id = integerFromApiString<arx::vehicle_id_t>(payload["attacker_vehicle_id"]);
    }
    else if (payload.contains("vehicle_id")) {
        vehicle_id = integerFromApiString<arx::vehicle_id_t>(payload["vehicle_id"]);
    }
    ps2::Vehicle vehicle = ps2::Vehicle::None;
    ps2::vehicle_from_vehicle_id(vehicle_id, &vehicle);
    // Zone
    arx::zone_id_t zone_id = integerFromApiString<arx::zone_id_t>(payload["zone_id"]);
    ps2::Zone zone = state_factory_.getZone();
    if (ps2::zone_from_zone_id(zone_id, &zone)) {
        qWarning() << "Unable to get zone from zone ID:" << zone_id;
    }
    // Update state factory
    if (are_we_the_baddies && vehicle != ps2::Vehicle::None) {
        state_factory_.setProfile(vehicle);
    }
    else {
        state_factory_.setProfile(class_);
    }
    state_factory_.setTeam(team);
    state_factory_.setZone(zone);
}

void ActivityTracker::handleGainexperiencePayload(const arx::json_t& payload) {
    bool wonders_of_modern_medicine = integerFromApiString<arx::character_id_t>(payload["character_id"]) == character_.id_;
    if (!wonders_of_modern_medicine) {
        // The character receiving experience is not the tracked character.
        // Since we do not discriminate between experience types yet, we
        // don't really learn anything from this payload.
        return;
    }
    // Class
    arx::loadout_id_t loadout_id = integerFromApiString<arx::loadout_id_t>(payload["loadout_id"]);
    ps2::Class class_ = state_factory_.getProfileAsClass();
    if (ps2::class_from_loadout_id(loadout_id, &class_)) {
        qWarning() << "Unable to get class from loadout ID:" << loadout_id;
        return; // Do not update state if we cannot tell what class we are
    }

    // TODO: We could use the experience type itself to make further
    // guesses about the character's activity, such as gunner or pilot
    // assists.

    // Zone
    arx::zone_id_t zone_id = integerFromApiString<arx::zone_id_t>(payload["zone_id"]);
    ps2::Zone zone = state_factory_.getZone();
    if (ps2::zone_from_zone_id(zone_id, &zone)) {
        qWarning() << "Unable to get zone from zone ID:" << zone_id;
    }
    // Update state factory
    if (state_factory_.getProfileAsVehicle() == ps2::Vehicle::None) {
        // We only override the profile with the current class if we're
        // already tracking as an infantry class, since we cannot tell if
        // the player is still in a vehicle from the experience tick alone.
        state_factory_.setProfile(class_);
    }
    state_factory_.setZone(zone);
}

QList<arx::Subscription> ActivityTracker::generateSubscriptions() const {
    auto deaths = arx::Subscription(
        // Event names
        { "Death" },
        // Characters
        { QString::number(character_.id_).toStdString() });
    auto experience = arx::Subscription(
        // Event names
        { "GainExperience" },
        // Characters
        { QString::number(character_.id_).toStdString() });
    return QList{ deaths, experience };
}

} // namespace PresenceApp

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning(push)
#   pragma warning(disable : 4464)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#endif

#include "moc_tracker.cpp"

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif
