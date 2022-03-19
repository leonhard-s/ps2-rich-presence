// Copyright 2022 Leonhard S.

#include "arx/ess/subs.hpp"

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "arx/types.hpp"

namespace arx
{

    Subscription::Subscription(const QString &event_name)
        : logical_and_{false},
          event_names_{{event_name}},
          character_ids_{{"all"}},
          world_ids_{{"all"}} {}

    Subscription::Subscription(const QStringList &event_names)
        : logical_and_{false},
          event_names_{event_names},
          character_ids_{{"all"}},
          world_ids_{{"all"}} {}

    Subscription::Subscription(const QString &event_name,
                               const QStringList &character_ids,
                               const QStringList &world_ids,
                               bool logical_and)
        : logical_and_{logical_and},
          event_names_{{event_name}},
          character_ids_{character_ids},
          world_ids_{world_ids} {}

    Subscription::Subscription(const QStringList &event_names,
                               const QStringList &character_ids,
                               const QStringList &world_ids,
                               bool logical_and)
        : logical_and_{logical_and},
          event_names_{event_names},
          character_ids_{character_ids},
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

    json_object Subscription::buildSubscribePayload() const
    {
        json_object json = buildBasicPayload();
        json["service"] = "event";
        json["action"] = "subscribe";
        return json;
    }

    json_object Subscription::buildUnsubscribePayload() const
    {
        json_object json = buildBasicPayload();
        json["service"] = "event";
        json["action"] = "clearSubscribe";
        return json;
    }

    json_object Subscription::buildUnsubscribeAllPayload()
    {
        json_object json;
        json["service"] = "event";
        json["action"] = "clearSubscribe";
        json["all"] = "true";
        return json;
    }

    json_object Subscription::buildBasicPayload() const
    {
        json_object json;
        // Add event names
        json_array event_names;
        for (const auto &event_name : event_names_)
        {
            event_names.push_back(event_name.toStdString());
        }
        json["eventNames"] = event_names;
        // Add character ids
        json_array character_ids;
        for (const auto &character_id : character_ids_)
        {
            character_ids.push_back(character_id.toStdString());
        }
        if (!character_ids.empty())
        {
            json["characters"] = character_ids;
        }
        // Add world ids
        json_array world_ids;
        for (const auto &world_id : world_ids_)
        {
            world_ids.push_back(world_id.toStdString());
        }
        if (!world_ids.empty())
        {
            json["worlds"] = world_ids;
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
