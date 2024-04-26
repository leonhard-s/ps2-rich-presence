// Copyright 2022 Leonhard S.

#include "faction.hpp"

#include <string>

#include <arx/ps2-types.hpp>

namespace ps2 {

int faction_from_faction_id(arx::faction_id_t faction_id, Faction* faction) {
    switch (faction_id) {
    case 0:
        *faction = Faction::NS;
        break;
    case 1:
        *faction = Faction::VS;
        break;
    case 2:
        *faction = Faction::NC;
        break;
    case 3:
        *faction = Faction::TR;
        break;
    case 4:
        *faction = Faction::NSO;
        break;
    default:
        return -1;
    }
    return 0;
}

int faction_to_display_name(Faction faction, std::string* display_name) {
    switch (faction) {
    case Faction::NS:
        *display_name = "Nanite Systems";
        break;
    case Faction::VS:
        *display_name = "Vanu Sovereignty";
        break;
    case Faction::NC:
        *display_name = "New Conglomerate";
        break;
    case Faction::TR:
        *display_name = "Terran Republic";
        break;
    case Faction::NSO:
        *display_name = "Nanite Systems Operatives";
        break;
    default:
        return -1;
    }
    return 0;
}

int faction_to_tag(Faction faction, std::string* tag) {
    switch (faction) {
    case Faction::NS:
        *tag = "NS";
        break;
    case Faction::VS:
        *tag = "VS";
        break;
    case Faction::NC:
        *tag = "NC";
        break;
    case Faction::TR:
        *tag = "TR";
        break;
    case Faction::NSO:
        *tag = "NSO";
        break;
    default:
        return -1;
    }
    return 0;
}

arx::faction_id_t faction_to_faction_id(Faction faction) {
    switch (faction) {
    case Faction::VS:
        return 1;
    case Faction::NC:
        return 2;
    case Faction::TR:
        return 3;
    case Faction::NSO:
        return 4;
    case Faction::NS:
    default:
        return 0;
    }
}

} // namespace ps2
