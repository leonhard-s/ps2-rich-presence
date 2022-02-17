// Copyright 2022 Leonhard S.

#include "game/character-info.hpp"

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

    CharacterInfo::CharacterInfo(QObject *parent)
        : QObject(parent), id_{0}, name_{}, faction_{ps2::Faction::NS},
          class_{ps2::Class::LightAssault}, world_{ps2::Server::Connery}
    {
        manager_.reset(new QNetworkAccessManager(this));
    }

    CharacterInfo::CharacterInfo(ps2::CharacterId id, QObject *parent)
        : CharacterInfo(parent)
    {
        id_ = id;
    }

    CharacterInfo::CharacterInfo(ps2::CharacterId id,
                                 const QString &name,
                                 ps2::Faction faction,
                                 ps2::Class class_,
                                 ps2::Server world,
                                 QObject *parent)
        : CharacterInfo(id, parent)
    {
        name_ = name;
        faction_ = faction;
        this->class_ = class_;
        world_ = world;
    }

    ps2::CharacterId CharacterInfo::getId() const
    {
        return id_;
    }

    QString CharacterInfo::getName() const
    {
        return name_;
    }

    ps2::Faction CharacterInfo::getFaction() const
    {
        return faction_;
    }

    ps2::Class CharacterInfo::getClass() const
    {
        return class_;
    }

    ps2::Server CharacterInfo::getWorld() const
    {
        return world_;
    }

    void CharacterInfo::populate()
    {
        // Only look up sensible character IDs
        if (id_ <= 0)
        {
            qWarning() << "Call to CharacterInfo::populate() ignored due to "
                          "invalid character ID:"
                       << id_;
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
        query.addTerm(arx::SearchTerm("character_id", std::to_string(id_)));
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
        ps2::Server new_world;
        if (ps2::server_from_world_id(world_id, new_world))
        {
            qWarning() << "Failed to convert world ID" << world_id;
            return;
        }
        // Update fields
        updateFieldsIfChanged(new_id, new_name, new_faction, new_class, new_world);
    }

    void CharacterInfo::updateFieldsIfChanged(ps2::CharacterId id,
                                              const QString &name,
                                              ps2::Faction faction,
                                              ps2::Class class_,
                                              ps2::Server world)
    {
        bool changed = false;
        if (id != id_)
        {
            id_ = id;
            changed = true;
        }
        if (name != name_)
        {
            name_ = name;
            changed = true;
        }
        if (faction != faction_)
        {
            faction_ = faction;
            changed = true;
        }
        if (class_ != this->class_)
        {
            this->class_ = class_;
            changed = true;
        }
        if (world != world_)
        {
            world_ = world;
            changed = true;
        }
        // Emit signal if any fields changed
        if (changed)
        {
            emit infoChanged();
        }
    }

} // namespace ps2rpc
