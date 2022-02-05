// Copyright 2022 Leonhard S.

#include "ps2.hpp"

namespace ps2
{

    int class_from_loadout_id(int loadout_id, Class &class_)
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
        case 2:  // NC
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

    int class_from_profile_id(int profile_id, Class &class_)
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

    int faction_from_faction_id(int faction_id, Faction &faction)
    {
        switch (faction_id)
        {
        case 0:
            faction = Faction::NS;
            break;
        case 1:
            faction = Faction::VS;
            break;
        case 2:
            faction = Faction::NC;
            break;
        case 3:
            faction = Faction::TR;
            break;
        case 4:
            faction = Faction::NSO;
            break;
        default:
            return -1;
        }
        return 0;
    }

    int faction_to_display_name(Faction faction, std::string &display_name)
    {
        switch (faction)
        {
        case Faction::NS:
            display_name = "Nanite Systems";
            break;
        case Faction::VS:
            display_name = "Vanu Sovereignty";
            break;
        case Faction::NC:
            display_name = "New Conglomerate";
            break;
        case Faction::TR:
            display_name = "Terran Republic";
            break;
        case Faction::NSO:
            display_name = "Nanite Systems Operatives";
            break;
        default:
            return -1;
        }
        return 0;
    }

    int faction_to_tag(Faction faction, std::string &tag)
    {
        switch (faction)
        {
        case Faction::NS:
            tag = "NS";
            break;
        case Faction::VS:
            tag = "VS";
            break;
        case Faction::NC:
            tag = "NC";
            break;
        case Faction::TR:
            tag = "TR";
            break;
        case Faction::NSO:
            tag = "NSO";
            break;
        default:
            return -1;
        }
        return 0;
    }

    int vehicle_from_vehicle_id(int vehicle_id, Vehicle &vehicle)
    {
        switch (vehicle_id)
        {
        case 0:
            vehicle = Vehicle::None;
            break;
        case 1:
            vehicle = Vehicle::Flash;
            break;
        case 2:
            vehicle = Vehicle::Sunderer;
            break;
        case 3:
            vehicle = Vehicle::Lightning;
            break;
        case 4:
            vehicle = Vehicle::Magrider;
            break;
        case 5:
            vehicle = Vehicle::Vanguard;
            break;
        case 6:
            vehicle = Vehicle::Prowler;
            break;
        case 7:
            vehicle = Vehicle::Scythe;
            break;
        case 8:
            vehicle = Vehicle::Reaver;
            break;
        case 9:
            vehicle = Vehicle::Mosquito;
            break;
        case 10:
            vehicle = Vehicle::Liberator;
            break;
        case 11:
            vehicle = Vehicle::Galaxy;
            break;
        case 12:
            vehicle = Vehicle::Harasser;
            break;
        case 14:
            vehicle = Vehicle::Valkyrie;
            break;
        case 15:
            vehicle = Vehicle::Ant;
            break;
        case 100:
            vehicle = Vehicle::AiTurret;
            break;
        case 150:
            vehicle = Vehicle::AaTurret;
            break;
        case 151:
            vehicle = Vehicle::AvTurret;
            break;
        case 2007:
            vehicle = Vehicle::Colossus;
            break;
        case 2019:
            vehicle = Vehicle::Bastion;
            break;
        case 2033:
        case 2125:
        case 2129:
            vehicle = Vehicle::Javelin;
            break;
        case 2122:
        case 2023:
        case 2124:
            vehicle = Vehicle::Interceptor;
            break;
        case 2136:
            vehicle = Vehicle::Dervish;
            break;
        default:
            return -1;
        }
        return 0;
    }

    int vehicle_to_display_name(Vehicle vehicle, std::string &display_name)
    {
        switch (vehicle)
        {
        case Vehicle::Flash:
            display_name = "Flash";
            break;
        case Vehicle::Sunderer:
            display_name = "Sunderer";
            break;
        case Vehicle::Lightning:
            display_name = "Lightning";
            break;
        case Vehicle::Magrider:
            display_name = "Magrider";
            break;
        case Vehicle::Vanguard:
            display_name = "Vanguard";
            break;
        case Vehicle::Prowler:
            display_name = "Prowler";
            break;
        case Vehicle::Scythe:
            display_name = "Scythe";
            break;
        case Vehicle::Reaver:
            display_name = "Reaver";
            break;
        case Vehicle::Mosquito:
            display_name = "Mosquito";
            break;
        case Vehicle::Liberator:
            display_name = "Liberator";
            break;
        case Vehicle::Galaxy:
            display_name = "Galaxy";
            break;
        case Vehicle::Harasser:
            display_name = "Harasser";
            break;
        case Vehicle::Valkyrie:
            display_name = "Valkyrie";
            break;
        case Vehicle::Ant:
            display_name = "Ant";
            break;
        case Vehicle::AiTurret:
            display_name = "AI Turret";
            break;
        case Vehicle::AaTurret:
            display_name = "AA Turret";
            break;
        case Vehicle::AvTurret:
            display_name = "AV Turret";
            break;
        case Vehicle::Colossus:
            display_name = "Colossus";
            break;
        case Vehicle::Bastion:
            display_name = "Bastion";
            break;
        case Vehicle::Javelin:
            display_name = "Javelin";
            break;
        case Vehicle::Interceptor:
            display_name = "Interceptor";
            break;
        case Vehicle::Dervish:
            display_name = "Dervish";
            break;
        default:
            return -1;
        }
        return 0;
    }

    int server_from_world_id(int world_id, Server &server)
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

    int zone_from_zone_id(int zone_id, Zone &zone)
    {
        switch (zone_id)
        {
        case 2:
            zone = Zone::Indar;
            break;
        case 4:
            zone = Zone::Hossin;
            break;
        case 6:
            zone = Zone::Amerish;
            break;
        case 8:
            zone = Zone::Esamir;
            break;
        case 14:
            zone = Zone::Koltyr;
            break;
        case 96: // NC
        case 97: // TR
        case 98: // VS
            zone = Zone::VrTraining;
            break;
        case 344:
            zone = Zone::Oshur;
            break;
        case 361:
            zone = Zone::Desolation;
            break;
        case 362:
            zone = Zone::Sanctuary;
            break;
        case 364:
            zone = Zone::Tutorial;
            break;
        default:
            return -1;
        }
        return 0;
    }

    int zone_to_display_name(Zone zone, std::string &display_name)
    {
        switch (zone)
        {
        case Zone::Indar:
            display_name = "Indar";
            break;
        case Zone::Hossin:
            display_name = "Hossin";
            break;
        case Zone::Amerish:
            display_name = "Amerish";
            break;
        case Zone::Esamir:
            display_name = "Esamir";
            break;
        case Zone::Oshur:
            display_name = "Oshur";
            break;
        case Zone::Sanctuary:
            display_name = "Sanctuary";
            break;
        default:
            return -1;
        }
        return 0;
    }

} // namespace ps2
