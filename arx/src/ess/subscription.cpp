// Copyright 2022 Leonhard S.

#include "arx/ess/subscription.hpp"

#include <vector>

#include "arx/types.hpp"

namespace
{

    arx::json_t subscriptionBoilerplate()
    {
        arx::json_t data;
        data["service"] = "event";
        data["action"] = "subscribe";
        return data;
    }

} // namespace

namespace arx
{

    Subscription::Subscription(std::vector<string_t> event_names,
                               std::vector<string_t> characters,
                               std::vector<string_t> worlds,
                               bool logical_and)
        : event_names_{event_names}, characters_{characters},
          worlds_{worlds}, logical_and_{logical_and} {}

    bool Subscription::operator==(const Subscription &other) const
    {
        return event_names_ == other.event_names_ &&
               characters_ == other.characters_ &&
               worlds_ == other.worlds_ &&
               logical_and_ == other.logical_and_;
    }

    bool Subscription::operator!=(const Subscription &other) const
    {
        return !(*this == other);
    }

    string_t Subscription::buildSubscribeMessage() const
    {
        json_t data = subscriptionBoilerplate();
        data["eventNames"] = buildEventNameList();
        if (!characters_.empty())
        {
            data["characters"] = buildCharacterList();
        }
        if (!worlds_.empty())
        {
            data["worlds"] = buildWorldList();
        }
        if (logical_and_)
        {
            data["logicalAndCharactersWithWorlds"] = true;
        }
        return data.dump();
    }

    string_t Subscription::buildUnsubscribeMessage() const
    {
        json_t data;
        data["service"] = "event";
        data["action"] = "clearSubscribe";
        data["eventNames"] = buildEventNameList();
        if (!characters_.empty())
        {
            data["characters"] = buildCharacterList();
        }
        if (!worlds_.empty())
        {
            data["eventNames"] = buildWorldList();
        }
        if (logical_and_)
        {
            data["logicalAndCharactersWithWorlds"] = true;
        }
        return data.dump();
    }

    const std::vector<string_t> Subscription::getEventNames() const
    {
        return event_names_;
    }

    const std::vector<string_t> Subscription::getCharacters() const
    {
        return characters_;
    }

    const std::vector<string_t> Subscription::getWorlds() const
    {
        return worlds_;
    }

    bool Subscription::getLogicalAndFlag() const
    {
        return logical_and_;
    }

    json_array_t Subscription::buildCharacterList() const
    {
        json_array_t data;
        for (const auto &character : characters_)
        {
            data.push_back(character);
        }
        return data;
    }

    json_array_t Subscription::buildEventNameList() const
    {
        json_array_t data;
        for (const auto &event : event_names_)
        {
            data.push_back(event);
        }
        return data;
    }

    json_array_t Subscription::buildWorldList() const
    {
        json_array_t data;
        for (const auto &world : worlds_)
        {
            data.push_back(world);
        }
        return data;
    }

} // namespace arx
