// Copyright 2022 Leonhard S.

#pragma once

#include "arx/types.hpp"

namespace arx {

/**
 * Enumeration of message types that may be received from the ESS.
 *
 * Heartbeat messages are received regularly from all online endpoints to
 * show they're active.
 *
 * Service messages are sent for event subscriptions and other one-off
 * commands like the "recentCharacterIds" action.
 *
 * Subscription echos are sent when adding or removing subscriptions.
 *
 * All other messages are treated via the "OTHER" type. This includes the
 * initial help message sent when connecting, the detailed on-demand help
 * message, and any "echo" command responses that do not mirror any of the
 * previous message types.
 */
enum class MessageType {
    HEARTBEAT,         // Regular heartbeat message
    SERVICE_MESSAGE,   // Event subscriptions and other service messages
    SUBSCRIPTION_ECHO, // Subscription confirmation echo
    OTHER              // Fallback value for generíc/unknown message types
};

/**
 * Determine the message type of the given message.
 *
 * @param message The message to determine the type of.
 * @return The message type of the given message.
 */
MessageType getMessageType(const json_t& message);

/**
 * Extract the payload from the given message.
 *
 * For messages of any type other than SERVICE_MESSAGE, this will return an
 * empty object.
 *
 * @param message The message to extract the payload from.
 * @return The payload of the given message.
 */
json_t getPayload(const json_t& message);

} // namespace arx
