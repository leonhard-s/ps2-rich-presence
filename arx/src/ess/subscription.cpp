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
                               bool logical_and = false)
        : event_names_{event_names}, characters_{characters},
          worlds_{worlds}, logical_and_{logical_and} {}

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
            data["eventNames"] = buildWorldList();
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
        return json_array_t(characters_.begin(), characters_.end());
    }

    json_array_t Subscription::buildEventNameList() const
    {
        return json_array_t(event_names_.begin(), event_names_.end());
    }

    json_array_t Subscription::buildWorldList() const
    {
        return json_array_t(worlds_.begin(), worlds_.end());
    }

} // namespace arx
