// Copyright 2022 Leonhard S.

#include "tracker.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
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
#include "ess_client.hpp"
#include "game/character-info.hpp"
#include "game/state.hpp"
#include "utils.hpp"

namespace ps2rpc
{

    ActivityTracker::ActivityTracker(const CharacterData &character, QObject *parent)
        : QObject(parent), character_{character}, state_factory_{character.id, character.faction, character.server, character.class_},
          current_state_{}, ess_client_{}
    {
        // Create WebSocket client for event streaming endpoint
        ess_client_.reset(new EssClient(SERVICE_ID, this));
        ess_client_->subscribe(generateSubscription());
        ess_client_->connect();
        QObject::connect(ess_client_.get(), &EssClient::payloadReceived,
                         this, &ActivityTracker::onPayloadReceived);
    }

    CharacterData ActivityTracker::getCharacter() const
    {
        return character_;
    }

    void ActivityTracker::onPayloadReceived(const QString &event_name,
                                            const arx::json_t &arx_payload)
    {
        // HACK: Replace payload with Qt JSON version
        auto doc = QJsonDocument::fromJson(arx_payload.dump().c_str());
        auto payload = doc.object();
        emit payloadReceived(event_name, payload);
        // Character ID
        bool are_we_the_baddies = payload["attacker_character_id"]
                                      .toString()
                                      .toULongLong() == character_.id;
        // Team
        // TODO: Implement team ID once it is implemented on the API side
        ps2::Faction team = state_factory_.getFaction();
        // Class
        arx::loadout_id_t loadout_id = are_we_the_baddies ? payload["attacker_loadout_id"].toString().toInt()
                                                          : payload["character_loadout_id"].toString().toInt();
        ps2::Class class_;
        if (ps2::class_from_loadout_id(loadout_id, class_))
        {
            qWarning() << "Unable to get class from loadout ID:" << loadout_id;
        }
        // Vehicle
        arx::vehicle_id_t vehicle_id = are_we_the_baddies ? payload["attacker_vehicle_id"].toString().toInt()
                                                          : payload["vehicle_id"].toString().toInt();
        ps2::Vehicle vehicle = ps2::Vehicle::None;
        ps2::vehicle_from_vehicle_id(vehicle_id, vehicle);
        // Zone
        arx::zone_id_t zone_id = payload["zone_id"].toString().toInt();
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

    arx::Subscription ActivityTracker::generateSubscription() const
    {
        auto sub = arx::Subscription(
            // Event names
            {"Death"},
            // Characters
            {QString::number(character_.id).toStdString()});
        return sub;
    }

} // namespace ps2rpc
