// Copyright 2022 Leonhard S.

#pragma once

#include "arx/types.hpp"

namespace arx {

/**
 * Validate the given Census API payload.
 *
 * @param collection The collection name to validate for.
 * @param payload The payload to validate.
 * @return 0 if the payload is valid, -1 if the payload contains an error,
 * -2 if the payload is missing a required key, and -3 if the return list
 * key is not a JSON array.
 */
int validatePayload(const json_string_t& collection,
    const json_t& payload);

/**
 * Check whether the given JSON payload return list is empty.
 *
 * @param collection The collection name to check for.
 * @param payload The payload to check.
 * @return True if the payload return list is empty, false otherwise.
 */
bool isPayloadEmpty(const json_string_t& collection,
    const json_t& payload);

/**
 * Create a JSON object from the given serialised payload.
 *
 * @param data JSON payload to deserialise.
 * @return A JSON object containing the deserialised payload.
 */
json_t parseJson(const json_string_t& data);

/**
 * Retrieve the first result list entry from the given payload.
 *
 * @param collection The collection name of the payload.
 * @param payload The payload to retrieve the first result from.
 * @return The first result list entry from the given payload.
 */
json_t payloadResultAsObject(const json_string_t& collection,
    const json_t& payload);

/**
 * Retrieve the result list from the given payload.
 *
 * @param collection The collection name of the payload.
 * @param payload The payload to retrieve the result list from.
 * @return The result list from the given payload.
 */
json_array_t payloadResultsAsArray(const json_string_t& collection,
    const json_t& payload);

} // namespace arx
