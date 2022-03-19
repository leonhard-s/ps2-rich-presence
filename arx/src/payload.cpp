// Copyright 2022 Leonhard S.

#include "arx/payload.hpp"

#include <string>

#include "arx/types.hpp"

namespace
{

    std::string getResultListName(const std::string &collection)
    {
        return collection + "_list";
    }

} // namespace

namespace arx
{

    int validatePayload(const json_string &collection, const json_object &payload)
    {
        if (payload.contains("error") || payload.contains("errorCode"))
        {
            return -1; // Response contains error, not valid
        }
        if (!payload.contains("returned") ||
            !payload.contains(getResultListName(collection)))
        {
            return -2; // Payload is missing a required key
        }
        if (!payload["returned"].is_array())
        {
            return -3; // Return list key is not an array
        }
        return 0;
    }

    bool isPayloadEmpty(const json_string &collection, const json_object &payload)
    {
        if (payload["returned"] == "0")
        {
            return true;
        }
        auto key = getResultListName(collection);
        if (payload.contains(key))
        {
            return payload[key].empty();
        }
        return false;
    }

    json_object payloadResultAsObject(const json_string &collection,
                                      const json_object &payload)
    {
        auto key = getResultListName(collection);
        return payload[key][0];
    }

    json_array payloadResultsAsArray(const json_string &collection,
                                     const json_object &payload)
    {
        auto key = getResultListName(collection);
        return payload[key];
    }

} // namespace arx
