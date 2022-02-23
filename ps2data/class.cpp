// Copyright 2022 Leonhard S.

#include "class.hpp"

#include <string>

#include "id-types.hpp"

namespace ps2
{

    int class_from_loadout_id(LoadoutId loadout_id, Class &class_)
    {
        switch (loadout_id)
        {
        // Infiltrator
        case 1:  // NC
        case 8:  // TR
        case 15: // VS
        case 28: // NSO
            class_ = Class::Infiltrator;
            break;
        // Light Assault
        case 3:  // NC
        case 10: // TR
        case 17: // VS
        case 29: // NSO
            class_ = Class::LightAssault;
            break;
        // Combat Medic
        case 4:  // NC
        case 11: // TR
        case 18: // VS
        case 30: // NSO
            class_ = Class::CombatMedic;
            break;
        // Engineer
        case 5:  // NC
        case 12: // TR
        case 19: // VS
        case 31: // NSO
            class_ = Class::Engineer;
            break;
        // Heavy Assault
        case 6:  // NC
        case 13: // TR
        case 20: // VS
        case 32: // NSO
            class_ = Class::HeavyAssault;
            break;
        // MAX
        case 7:  // NC
        case 14: // TR
        case 21: // VS
        case 45: // NSO
            class_ = Class::MAX;
            break;
        default:
            return -1;
        }
        return 0;
    }

    int class_from_profile_id(ProfileId profile_id, Class &class_)
    {
        switch (profile_id)
        {
        // Infiltrator
        case 2:   // NC
        case 10:  // TR
        case 17:  // VS
        case 190: // NSO
            class_ = Class::Infiltrator;
            break;
        // Light Assault
        case 4:   // NC
        case 12:  // TR
        case 19:  // VS
        case 191: // NSO
            class_ = Class::LightAssault;
            break;
        // Combat Medic
        case 5:   // NC
        case 13:  // TR
        case 20:  // VS
        case 192: // NSO
            class_ = Class::CombatMedic;
            break;
        // Engineer
        case 6:   // NC
        case 14:  // TR
        case 21:  // VS
        case 193: // NSO
            class_ = Class::Engineer;
            break;
        // Heavy Assault
        case 7:   // NC
        case 15:  // TR
        case 22:  // VS
        case 194: // NSO
            class_ = Class::HeavyAssault;
            break;
        // MAX
        case 8:   // NC
        case 16:  // TR
        case 23:  // VS
        case 252: // NSO
            class_ = Class::MAX;
            break;
        default:
            return -1;
        }
        return 0;
    }

    int class_to_display_name(Class class_, std::string &display_name)
    {
        switch (class_)
        {
        case Class::Infiltrator:
            display_name = "Infiltrator";
            break;
        case Class::LightAssault:
            display_name = "Light Assault";
            break;
        case Class::CombatMedic:
            display_name = "Combat Medic";
            break;
        case Class::Engineer:
            display_name = "Engineer";
            break;
        case Class::HeavyAssault:
            display_name = "Heavy Assault";
            break;
        case Class::MAX:
            display_name = "MAX";
            break;
        default:
            return -1;
        }
        return 0;
    }

} // namespace ps2
