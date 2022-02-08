// Copyright 2022 Leonhard S.

#include "appdata/assets.hpp"

#include <string>

#include "ps2.hpp"

namespace assets
{
    int imageKeyFromClass(ps2::Class class_, std::string &image_key)
    {
        switch (class_)
        {
        case ps2::Class::Infiltrator:
            image_key = "infiltrator";
            break;
        case ps2::Class::LightAssault:
            image_key = "light_assault";
            break;
        case ps2::Class::CombatMedic:
            image_key = "combat_medic";
            break;
        case ps2::Class::Engineer:
            image_key = "engineer";
            break;
        case ps2::Class::HeavyAssault:
            image_key = "heavy_assault";
            break;
        case ps2::Class::MAX:
            image_key = "max";
            break;
        default:
            return -1;
        }
        return 0;
    }

    int imageKeyFromVehicle(ps2::Vehicle vehicle, std::string &image_key)
    {
        switch (vehicle)
        {
        case ps2::Vehicle::Flash:
            image_key = "flash";
            break;
        case ps2::Vehicle::Sunderer:
            image_key = "sunderer";
            break;
        case ps2::Vehicle::Lightning:
            image_key = "lightning";
            break;
        case ps2::Vehicle::Magrider:
            image_key = "magrider";
            break;
        case ps2::Vehicle::Vanguard:
            image_key = "vanguard";
            break;
        case ps2::Vehicle::Prowler:
            image_key = "prowler";
            break;
        case ps2::Vehicle::Scythe:
            image_key = "scythe";
            break;
        case ps2::Vehicle::Reaver:
            image_key = "reaver";
            break;
        case ps2::Vehicle::Mosquito:
            image_key = "mosquito";
            break;
        case ps2::Vehicle::Liberator:
            image_key = "liberator";
            break;
        case ps2::Vehicle::Galaxy:
            image_key = "galaxy";
            break;
        case ps2::Vehicle::Harasser:
            image_key = "harasser";
            break;
        case ps2::Vehicle::Valkyrie:
            image_key = "valkyrie";
            break;
        case ps2::Vehicle::Ant:
            image_key = "ant";
            break;
        case ps2::Vehicle::AiTurret:
            image_key = "ai_turret";
            break;
        case ps2::Vehicle::AaTurret:
            image_key = "aa_turret";
            break;
        case ps2::Vehicle::AvTurret:
            image_key = "av_turret";
            break;
        case ps2::Vehicle::Colossus:
            image_key = "colossus";
            break;
        case ps2::Vehicle::Bastion:
            image_key = "bastion";
            break;
        case ps2::Vehicle::Javelin:
            image_key = "javelin";
            break;
        case ps2::Vehicle::Interceptor:
            image_key = "interceptor";
            break;
        case ps2::Vehicle::Dervish:
            image_key = "dervish";
            break;
        default:
            return -1;
        }
        return 0;
    }

    int imageKeyFromZone(ps2::Zone zone, std::string &image_key)
    {
        switch (zone)
        {
        case ps2::Zone::Indar:
            image_key = "zone_indar";
            break;
        case ps2::Zone::Hossin:
            image_key = "zone_hossin";
            break;
        case ps2::Zone::Amerish:
            image_key = "zone_amerish";
            break;
        case ps2::Zone::Esamir:
            image_key = "zone_esamir";
            break;
        case ps2::Zone::Oshur:
            image_key = "zone_oshur";
            break;
        default:
            return -1;
        }
        return 0;
    }

} // namespace assets
