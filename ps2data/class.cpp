// Copyright 2022 Leonhard S.

#include "class.hpp"

#include <string>

#include "arx/ps2-types.hpp"

#include "faction.hpp"

namespace ps2 {

int class_from_loadout_id(arx::loadout_id_t loadout_id, Class* cls) {
    switch (loadout_id) {
        // Infiltrator
    case 1:  // NC
    case 8:  // TR
    case 15: // VS
    case 28: // NSO
        *cls = Class::Infiltrator;
        break;
        // Light Assault
    case 3:  // NC
    case 10: // TR
    case 17: // VS
    case 29: // NSO
        *cls = Class::LightAssault;
        break;
        // Combat Medic
    case 4:  // NC
    case 11: // TR
    case 18: // VS
    case 30: // NSO
        *cls = Class::CombatMedic;
        break;
        // Engineer
    case 5:  // NC
    case 12: // TR
    case 19: // VS
    case 31: // NSO
        *cls = Class::Engineer;
        break;
        // Heavy Assault
    case 6:  // NC
    case 13: // TR
    case 20: // VS
    case 32: // NSO
        *cls = Class::HeavyAssault;
        break;
        // MAX
    case 7:  // NC
    case 14: // TR
    case 21: // VS
    case 45: // NSO
        *cls = Class::MAX;
        break;
    default:
        return -1;
    }
    return 0;
}

int class_from_profile_id(arx::profile_id_t profile_id, Class* cls) {
    switch (profile_id) {
        // Infiltrator
    case 2:   // NC
    case 10:  // TR
    case 17:  // VS
    case 190: // NSO
        *cls = Class::Infiltrator;
        break;
        // Light Assault
    case 4:   // NC
    case 12:  // TR
    case 19:  // VS
    case 191: // NSO
        *cls = Class::LightAssault;
        break;
        // Combat Medic
    case 5:   // NC
    case 13:  // TR
    case 20:  // VS
    case 192: // NSO
        *cls = Class::CombatMedic;
        break;
        // Engineer
    case 6:   // NC
    case 14:  // TR
    case 21:  // VS
    case 193: // NSO
        *cls = Class::Engineer;
        break;
        // Heavy Assault
    case 7:   // NC
    case 15:  // TR
    case 22:  // VS
    case 194: // NSO
        *cls = Class::HeavyAssault;
        break;
        // MAX
    case 8:   // NC
    case 16:  // TR
    case 23:  // VS
    case 252: // NSO
        *cls = Class::MAX;
        break;
    default:
        return -1;
    }
    return 0;
}

int class_to_display_name(Class cls, std::string* display_name) {
    switch (cls) {
    case Class::Infiltrator:
        *display_name = "Infiltrator";
        break;
    case Class::LightAssault:
        *display_name = "Light Assault";
        break;
    case Class::CombatMedic:
        *display_name = "Combat Medic";
        break;
    case Class::Engineer:
        *display_name = "Engineer";
        break;
    case Class::HeavyAssault:
        *display_name = "Heavy Assault";
        break;
    case Class::MAX:
        *display_name = "MAX";
        break;
    default:
        return -1;
    }
    return 0;
}

arx::profile_id_t class_to_profile_id(Class cls, Faction faction) {
    switch (cls) {
    case Class::Infiltrator:
        switch (faction) {
        case Faction::NC:
            return 2;
        case Faction::TR:
            return 10;
        case Faction::VS:
            return 17;
        case Faction::NSO:
            return 190;
        case Faction::NS:
        default:
            return 0;
        }
    case Class::LightAssault:
        switch (faction) {
        case Faction::NC:
            return 4;
        case Faction::TR:
            return 12;
        case Faction::VS:
            return 19;
        case Faction::NSO:
            return 191;
        case Faction::NS:
        default:
            return 0;
        }
    case Class::CombatMedic:
        switch (faction) {
        case Faction::NC:
            return 5;
        case Faction::TR:
            return 13;
        case Faction::VS:
            return 20;
        case Faction::NSO:
            return 192;
        case Faction::NS:
        default:
            return 0;
        }
    case Class::Engineer:
        switch (faction) {
        case Faction::NC:
            return 6;
        case Faction::TR:
            return 14;
        case Faction::VS:
            return 21;
        case Faction::NSO:
            return 193;
        case Faction::NS:
        default:
            return 0;
        }
    case Class::HeavyAssault:
        switch (faction) {
        case Faction::NC:
            return 7;
        case Faction::TR:
            return 15;
        case Faction::VS:
            return 22;
        case Faction::NSO:
            return 194;
        case Faction::NS:
        default:
            return 0;
        }
    case Class::MAX:
        switch (faction) {
        case Faction::NC:
            return 8;
        case Faction::TR:
            return 16;
        case Faction::VS:
            return 23;
        case Faction::NSO:
            return 252;
        case Faction::NS:
        default:
            return 0;
        }
    default:
        return 0;
    }
}

} // namespace ps2
