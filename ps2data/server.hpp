// Copyright 2022 Leonhard S.

#ifndef PS2DATA_SERVER_HPP
#define PS2DATA_SERVER_HPP

#include <string>

#include "id-types.hpp"

namespace ps2
{

    /** Enumeration of game servers in PlanetSide 2. */
    enum class Server
    {
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
    int server_from_world_id(WorldId world_id, Server &server);

    /**
     * Return the display name for a given server enum value.
     *
     * @param server The server enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int server_to_display_name(Server server, std::string &display_name);

} // namespace ps2

#endif // PS2DATA_SERVER_HPP
