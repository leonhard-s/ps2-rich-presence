// Copyright 2022 Leonhard S.

#include "arx/ess/payload.hpp"

#include "arx/types.hpp"

namespace
{

    arx::string_t getService(const arx::json_t &message)
    {
        auto it = message.find("service");
        if (it == message.end() || !it->is_string())
        {
            return "";
        }
        return it->get<arx::string_t>();
    }

    arx::MessageType getMessageTypeEvent(const arx::json_t &message)
    {
        auto it = message.find("type");
        if (it == message.end() || !it->is_string())
        {
            return arx::MessageType::OTHER; // Used by initial help message
        }
        auto type = it->get<arx::string_t>();
        if (type == "serviceMessage")
        {
            return arx::MessageType::SERVICE_MESSAGE;
        }
        if (type == "heartbeat")
        {
            return arx::MessageType::HEARTBEAT;
        }
        return arx::MessageType::OTHER;
    }

} // namespace

namespace arx
{

    MessageType getMessageType(const json_t &message)
    {
        auto service = getService(message);
        // Heartbeat & service messages
        if (service == "event")
        {
            return getMessageTypeEvent(message);
        }
        // Subscription echo
        if (service == "")
        {
            // Subscriptions do not have a service specified but must have a
            // top-level "subscription" key
            if (message.find("subscription") != message.end())
            {
                return MessageType::SUBSCRIPTION_ECHO;
            }
        }
        // Everything else
        return MessageType::OTHER;
    }

    json_t getPayload(const json_t &message)
    {
        if (getMessageType(message) != MessageType::SERVICE_MESSAGE)
        {
            return json_t();
        }
        auto it = message.find("payload");
        if (it == message.end() || !it->is_object())
        {
            return json_t();
        }
        auto payload = it->get<json_t>();
        if (payload.find("event_name") == payload.end())
        {
            return json_t();
        }
        return payload;
    }

} // namespace arx
