// Copyright 2022 Leonhard S.

#pragma once

#include <string>

#include "arx/ps2_types.hpp"

namespace ps2 {

/** Enumeration of game servers in PlanetSide 2. */
enum class Server {
    Cobalt,
    Connery,
    Emerald,
    Jaeger,
    Miller,
    SolTech,
};

/**
 * Return the server enum value for a given Census API world ID.
 *
 * @param world_id The Census API world ID.
 * @param server The server enum value to be populated.
 * @return 0 on success, -1 on failure.
 */
int server_from_world_id(arx::world_id_t world_id, Server& server);

/**
 * Return the display name for a given server enum value.
 *
 * @param server The server enum value.
 * @param display_name The display name to be populated.
 * @return 0 on success, -1 on failure.
 */
int server_to_display_name(Server server, std::string& display_name);

/**
 * Return the Census API world ID for a given server enum value.
 *
 * @param server The server enum value.
 * @return The Census API world ID.
 */
arx::world_id_t server_to_world_id(Server server);

} // namespace ps2
