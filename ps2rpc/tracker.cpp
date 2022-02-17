// Copyright 2022 Leonhard S.

#include "tracker.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
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

#include "appdata/serviceid.hpp"
#include "game/character-info.hpp"
#include "game/state.hpp"
#include "utils.hpp"

namespace ps2rpc
{

    ActivityTracker::ActivityTracker(ps2::CharacterId character_id)
        : character_id_{character_id}, ess_client_{}, state_factory_{character_id_, ps2::Faction::VS, ps2::Server::Cobalt}
    {
        // Create WebSocket client for event streaming endpoint
        ess_client_.reset(new arx::EssClient(SERVICE_ID, "ps2", this));
        QObject::connect(ess_client_.get(), &arx::EssClient::payloadReceived, this, &ActivityTracker::onPayloadReceived);
        // Create character info stub and populate it
        char_info_.reset(new CharacterInfo(character_id, this));
        QObject::connect(char_info_.get(), &CharacterInfo::infoChanged,
                         this, &ActivityTracker::onCharacterInfoUpdated);
    }

    void ActivityTracker::onPayloadReceived(const QString &event_name, const QJsonObject &payload)
    {
        // Character ID
        ps2::CharacterId character_id = character_id_;
        bool are_we_the_baddies = payload["attacker_character_id"].toString().toLongLong() == character_id;
        // Team
        // TODO: Implement team ID once it is implemented on the API side
        ps2::Faction team = state_factory_.getFaction();
        // Class
        ps2::LoadoutId loadout_id = are_we_the_baddies ? payload["attacker_loadout_id"].toString().toInt()
                                                       : payload["loaodut_id"].toString().toInt();
        ps2::Class class_;
        if (ps2::class_from_loadout_id(loadout_id, class_))
        {
            qWarning() << "Unable to get class from loadout ID:" << loadout_id;
        }
        // Vehicle
        ps2::VehicleId vehicle_id = are_we_the_baddies ? payload["attacker_vehicle_id"].toString().toInt()
                                                       : payload["vehicle_id"].toString().toInt();
        ps2::Vehicle vehicle = ps2::Vehicle::None;
        ps2::vehicle_from_vehicle_id(vehicle_id, vehicle);
        // Zone
        ps2::ZoneId zone_id = payload["zone_id"].toString().toInt();
        ps2::Zone zone;
        if (ps2::zone_from_zone_id(zone_id, zone))
        {
            qWarning() << "Unable to get zone from zone ID:" << zone_id;
        }
        // Update state factory
        if (are_we_the_baddies && vehicle != ps2::Vehicle::None)
        {
            state_factory_.setProfile(vehicle);
        }
        else
        {
            state_factory_.setProfile(class_);
        }
        state_factory_.setTeam(team);
        state_factory_.setZone(zone);
        // Check if the new state is different from the old state
        GameState state;
        if (state_factory_.buildState(state))
        {
            qWarning() << "Unable to build state from factory";
            return;
        }
        if (state != current_state_)
        {
            current_state_ = state;
            emit stateChanged(state);
        }
    }

    void ActivityTracker::onCharacterInfoUpdated()
    {
        if (ess_client_->isConnected())
        {
            // Clear existing subscriptions
            ess_client_->clearSubscriptions();
        }
        else
        {
            ess_client_->connect();
            // Set idle state until we see a payload
            GameState state;
            state_factory_.buildState(state);
            emit stateChanged(state);
        }
        // Resubscribe for the updated character info
        auto sub = generateSubscription();
        ess_client_->subscribe(sub);
    }

    arx::Subscription ActivityTracker::generateSubscription() const
    {
        return arx::Subscription(
            // Event name(s)
            "Death",
            // Character IDs
            {QString::number(character_id_)},
            // World IDs
            {"all"});
    }

} // namespace ps2rpc
