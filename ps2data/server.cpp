// Copyright 2022 Leonhard S.

#include "server.hpp"

#include <string>

#include "arx/ps2_types.hpp"

namespace ps2
{

    int server_from_world_id(arx::world_id_t world_id, Server &server)
    {
        switch (world_id)
        {
        case 1:
            server = Server::Connery;
            break;
        case 10:
            server = Server::Miller;
            break;
        case 13:
            server = Server::Cobalt;
            break;
        case 17:
            server = Server::Emerald;
            break;
        case 19:
            server = Server::Jaeger;
            break;
        case 40:
            server = Server::SolTech;
            break;
        default:
            return -1;
        }
        return 0;
    }

    int server_to_display_name(Server server, std::string &display_name)
    {
        switch (server)
        {
        case Server::Cobalt:
            display_name = "Cobalt";
            break;
        case Server::Connery:
            display_name = "Connery";
            break;
        case Server::Emerald:
            display_name = "Emerald";
            break;
        case Server::Jaeger:
            display_name = "Jaeger";
            break;
        case Server::Miller:
            display_name = "Miller";
            break;
        case Server::SolTech:
            display_name = "SolTech";
            break;
        default:
            return -1;
        }
        return 0;
    }

    arx::world_id_t server_to_world_id(Server server)
    {
        switch (server)
        {
        case Server::Connery:
            return 1;
        case Server::Miller:
            return 10;
        case Server::Cobalt:
            return 13;
        case Server::Emerald:
            return 17;
        case Server::Jaeger:
            return 19;
        case Server::SolTech:
            return 40;
        default:
            return 0;
        }
    }

} // namespace ps2
