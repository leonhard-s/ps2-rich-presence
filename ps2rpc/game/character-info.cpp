// Copyright 2022 Leonhard S.

#include "game/character-info.hpp"

#include <QtCore/QDebug>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "arx.hpp"
#include "ps2.hpp"

#include "appdata/serviceid.hpp"
#include "utils.hpp"

namespace ps2rpc
{

    CharacterData::CharacterData()
        : id{0},
          name{""},
          faction{ps2::Faction::NS},
          class_{ps2::Class::LightAssault},
          server{ps2::Server::Connery} {}

    CharacterData::CharacterData(ps2::CharacterId id,
                                 const QString &name,
                                 ps2::Faction faction,
                                 ps2::Class class_,
                                 ps2::Server server)
        : id{id},
          name{name},
          faction{faction},
          class_{class_},
          server{server} {}

    bool CharacterData::operator==(const CharacterData &other) const
    {
        return id == other.id &&
               name == other.name &&
               faction == other.faction &&
               class_ == other.class_ &&
               server == other.server;
    }

    bool CharacterData::operator!=(const CharacterData &other) const
    {
        return !(*this == other);
    }

    QDebug operator<<(QDebug dbg, const CharacterData &info)
    {
        std::string tag;
        ps2::faction_to_tag(info.faction, tag);
        return dbg << (info.name + "[" + QString::fromStdString(tag) + "]");
    }

    CharacterInfo::CharacterInfo(QObject *parent)
        : QObject(parent), info_{}
    {
        manager_.reset(new QNetworkAccessManager(this));
    }

    CharacterInfo::CharacterInfo(ps2::CharacterId id, QObject *parent)
        : CharacterInfo(parent)
    {
        info_.id = id;
    }

    CharacterInfo::CharacterInfo(ps2::CharacterId id,
                                 const QString &name,
                                 ps2::Faction faction,
                                 ps2::Class class_,
                                 ps2::Server server,
                                 QObject *parent)
        : CharacterInfo(id, parent)
    {
        info_.name = name;
        info_.faction = faction;
        info_.class_ = class_;
        info_.server = server;
    }

    ps2::CharacterId CharacterInfo::getId() const
    {
        return info_.id;
    }

    QString CharacterInfo::getName() const
    {
        return info_.name;
    }

    ps2::Faction CharacterInfo::getFaction() const
    {
        return info_.faction;
    }

    ps2::Class CharacterInfo::getClass() const
    {
        return info_.class_;
    }

    ps2::Server CharacterInfo::getServer() const
    {
        return info_.server;
    }

    void CharacterInfo::populate()
    {
        // Only look up sensible character IDs
        auto id = info_.id;
        if (id <= 0)
        {
            qWarning() << "Call to CharacterInfo::populate() ignored due to "
                          "invalid character ID:"
                       << id;
            return;
        }
        // Generate request
        auto request = getCharacterInfoRequest();
        // Reply object will be deleted by reply handler
        auto reply = manager_->get(request);
        QObject::connect(reply, &QNetworkReply::finished,
                         this, &CharacterInfo::onCharacterInfoRequestFinished);
    }

    void CharacterInfo::onCharacterInfoRequestFinished()
    {
        // Get reply object from caller
        auto reply = qobject_cast<QNetworkReply *>(QObject::sender());
        if (!reply)
        {
            qWarning() << "CharacterInfo::onCharacterInfoRequestFinished()"
                       << "Signal sender is not a QNetworkReply object.";
        }
        // Delete reply once control is returned to the event loop
        reply->deleteLater();
        // Check for network errors
        if (reply->error() != QNetworkReply::NoError)
        {
            qWarning() << "CharacterInfo::onCharacterInfoRequestFinished()"
                       << "Network error:" << reply->errorString();
            return;
        }
        // Get payload from reply
        auto payload = getJsonPayload(*reply);
        // Handle payload
        handleCharacterInfoPayload(payload);
    }

    QNetworkRequest CharacterInfo::getCharacterInfoRequest()
    {
        // Create Query via ARX
        arx::Query query("character", SERVICE_ID);
        query.addTerm(
            arx::SearchTerm("character_id", std::to_string(info_.id)));
        query.setShow(
            {"character_id", "name.first", "faction_id", "profile_id"});
        auto join = arx::JoinData("characters_world");
        join.show.push_back("world_id");
        query.addJoin(join);
        // Generate URL
        auto url = qUrlFromArxQuery(query);
        // Create request
        return QNetworkRequest(url);
    }

    void CharacterInfo::handleCharacterInfoPayload(const QJsonObject &payload)
    {
        if (!arx::isPayloadValid("character", payload))
        {
            qWarning() << "CharacterInfo::handleCharacterInfoPayload(): "
                          "Invalid JSON payload";
            return;
        }
        // Get character object
        auto data = arx::payloadResultAsObject("character", payload);
        // Get new fields from payload
        auto new_id = data["character_id"].toString().toLongLong();
        auto new_name = data["name"].toObject()["first"].toString();
        auto faction_id = data["faction_id"].toString().toInt();
        auto profile_id = data["profile_id"].toString().toInt();
        auto world_id = data["world"].toObject()["world_id"].toString().toInt();
        // Convert ps2-specific IDs
        ps2::Faction new_faction;
        if (ps2::faction_from_faction_id(faction_id, new_faction))
        {
            qWarning() << "Failed to convert faction ID" << faction_id;
            return;
        }
        ps2::Class new_class;
        if (ps2::class_from_profile_id(profile_id, new_class))
        {
            qWarning() << "Failed to get class from profile ID" << profile_id;
            return;
        }
        ps2::Server new_server;
        if (ps2::server_from_world_id(world_id, new_server))
        {
            qWarning() << "Failed to get server from world ID" << world_id;
            return;
        }
        // Update fields
        updateFieldsIfChanged(
            new_id, new_name, new_faction, new_class, new_server);
    }

    void CharacterInfo::updateFieldsIfChanged(ps2::CharacterId id,
                                              const QString &name,
                                              ps2::Faction faction,
                                              ps2::Class class_,
                                              ps2::Server server)
    {
        bool changed = false;
        if (id != info_.id)
        {
            info_.id = id;
            changed = true;
        }
        if (name != info_.name)
        {
            info_.name = name;
            changed = true;
        }
        if (faction != info_.faction)
        {
            info_.faction = faction;
            changed = true;
        }
        if (class_ != info_.class_)
        {
            info_.class_ = class_;
            changed = true;
        }
        if (server != info_.server)
        {
            info_.server = server;
            changed = true;
        }
        // Emit signal if any fields changed
        if (changed)
        {
            emit infoChanged();
        }
    }

} // namespace ps2rpc
