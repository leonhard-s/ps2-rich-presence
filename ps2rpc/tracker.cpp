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
        QObject::connect(this, &ActivityTracker::ready, this, &ActivityTracker::onCharacterInfoReady);
        // Create Network Access Manager for REST requests
        network_manager_.reset(new QNetworkAccessManager(this));
        QNetworkRequest request = getCharacterInfoRequest(character_id);
        auto reply = network_manager_->get(request); // Pointer to be deleted by reply handler
        QObject::connect(reply, &QNetworkReply::finished, this, &ActivityTracker::onCharacerRequestFinished);
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

    void ActivityTracker::onCharacerRequestFinished()
    {
        auto reply = qobject_cast<QNetworkReply *>(QObject::sender());
        if (reply->error() != QNetworkReply::NoError)
        {
            qWarning() << "Error occurred while getting character info:" << reply->errorString();
        }
        else
        {
            auto payload = getJsonPayload(*reply);
            onCharacterInfoReceived(payload);
        }
        // Clean up the reply object
        reply->deleteLater();
    }

    void ActivityTracker::onCharacterInfoReceived(const QJsonObject payload)
    {
        // Break up the outer list
        auto char_list = payload["character_list"].toArray();
        if (char_list.empty())
        {
            qWarning() << "Character list is empty";
            return;
        }
        auto char_obj = char_list.at(0).toObject();
        // Get the character ID
        auto character_id = char_obj["character_id"].toString().toLongLong();
        // Get the faction ID
        auto faction_id = char_obj["faction_id"].toString().toInt();
        // Get the last seen profile
        auto profile_id = char_obj["profile_id"].toString().toInt();
        // Process world join
        auto world_join = char_obj["character_id_join_characters_world"].toObject();
        auto world_id = world_join["world_id"].toString().toInt();
        // Set internal fields
        character_id_ = character_id;
        ps2::Faction faction;
        ps2::faction_from_faction_id(faction_id, faction);
        ps2::Server server;
        ps2::server_from_world_id(world_id, server);
        ps2::Class class_;
        ps2::class_from_profile_id(profile_id, class_);
        state_factory_.setFaction(faction);
        state_factory_.setTeam(faction);
        state_factory_.setServer(server);
        state_factory_.setProfile(class_);
        emit ready();
    }

    void ActivityTracker::onCharacterInfoReady()
    {
        auto sub = generateSubscription();
        ess_client_->subscribe(sub);
        if (!ess_client_->isConnected())
        {
            ess_client_->connect();
        }
        GameState state;
        state_factory_.buildState(state);
        emit stateChanged(state);
    }

    QNetworkRequest ActivityTracker::getCharacterInfoRequest(ps2::CharacterId character_id)
    {
        // Create Query via ARX
        arx::Query query("character", SERVICE_ID);
        query.addTerm(arx::SearchTerm("faction_id", std::to_string(character_id)));
        query.setShow({"faction_id", "profile_id", "character_id"});
        auto join = arx::JoinData("characters_world");
        join.show.push_back("name.en");
        query.addJoin(join);
        return QNetworkRequest(qUrlFromArxQuery(query));
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
