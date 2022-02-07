// Copyright 2022 Leonhard S.

#include "arx/ess/subs.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

namespace arx
{

    Subscription::Subscription(const QString &event_name)
        : character_ids_{{"all"}},
          event_names_{{event_name}},
          logical_and_{false},
          world_ids_{{"all"}} {}

    Subscription::Subscription(const QStringList &event_names)
        : character_ids_{{"all"}},
          event_names_{event_names},
          logical_and_{false},
          world_ids_{{"all"}} {}

    Subscription::Subscription(const QString &event_name,
                               const QStringList &character_ids,
                               const QStringList &world_ids,
                               bool logical_and)
        : character_ids_{character_ids},
          event_names_{{event_name}},
          logical_and_{logical_and},
          world_ids_{world_ids} {}

    Subscription::Subscription(const QStringList &event_names,
                               const QStringList &character_ids,
                               const QStringList &world_ids,
                               bool logical_and)
        : character_ids_{character_ids},
          event_names_{event_names},
          logical_and_{logical_and},
          world_ids_{world_ids} {}

    bool Subscription::operator==(const Subscription &other) const
    {
        return logical_and_ == other.logical_and_ &&
               event_names_ == other.event_names_ &&
               character_ids_ == other.character_ids_ &&
               world_ids_ == other.world_ids_;
    }

    bool Subscription::operator!=(const Subscription &other) const
    {
        return !(*this == other);
    }

    QStringList Subscription::getEventNames() const
    {
        return event_names_;
    }

    QStringList Subscription::getCharacterIds() const
    {
        return character_ids_;
    }

    QStringList Subscription::getWorldIds() const
    {
        return world_ids_;
    }

    bool Subscription::getLogicalAndFlag() const
    {
        return logical_and_;
    }

    QJsonObject Subscription::buildSubscribePayload() const
    {
        QJsonObject json = buildBasicPayload();
        json["service"] = "event";
        json["action"] = "subscribe";
        return json;
    }

    QJsonObject Subscription::buildUnsubscribePayload() const
    {
        QJsonObject json = buildBasicPayload();
        json["service"] = "event";
        json["action"] = "clearSubscribe";
        return json;
    }

    QJsonObject Subscription::buildUnsubscribeAllPayload()
    {
        QJsonObject json;
        json["service"] = "event";
        json["action"] = "clearSubscribe";
        json["all"] = "true";
        return json;
    }

    QJsonObject Subscription::buildBasicPayload() const
    {
        QJsonObject json;
        // Add event names
        QJsonArray event_names;
        for (const auto &event_name : event_names_)
        {
            event_names.append(event_name);
        }
        json["eventNames"] = event_names;
        // Add character ids
        QJsonArray character_ids;
        for (const auto &character_id : character_ids_)
        {
            character_ids.append(character_id);
        }
        if (!character_ids.isEmpty())
        {
            json["characterIds"] = character_ids;
        }
        // Add world ids
        QJsonArray world_ids;
        for (const auto &world_id : world_ids_)
        {
            world_ids.append(world_id);
        }
        if (!world_ids.isEmpty())
        {
            json["worldIds"] = world_ids;
        }
        // Add "logicalAndCharactersWithWorlds" flag
        if (logical_and_)
        {
            // NOTE: This is actually a boolean value according to the API
            // spec. Which makes it the only unquoted value in the entire ESS.
            json["logicalAndCharactersWithWorlds"] = true;
        }
        return json;
    }

} // namespace arx
