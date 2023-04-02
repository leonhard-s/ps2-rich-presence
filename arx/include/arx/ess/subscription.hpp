// Copyright 2022 Leonhard S.

#pragma once

#include <vector>

#include "arx/types.hpp"

namespace arx {

class Subscription
{
public:
    Subscription(std::vector<string_t> event_names,
        std::vector<string_t> characters = {},
        std::vector<string_t> worlds = {},
        bool logical_and = false);

    bool operator==(const Subscription& other) const;
    bool operator!=(const Subscription& other) const;

    string_t buildSubscribeMessage() const;
    string_t buildUnsubscribeMessage() const;

    const std::vector<string_t> getEventNames() const;
    const std::vector<string_t> getCharacters() const;
    const std::vector<string_t> getWorlds() const;
    bool getLogicalAndFlag() const;

private:
    json_array_t buildCharacterList() const;
    json_array_t buildEventNameList() const;
    json_array_t buildWorldList() const;

    bool logical_and_;
    std::vector<string_t> characters_;
    std::vector<string_t> event_names_;
    std::vector<string_t> worlds_;
};

} // namespace arx
