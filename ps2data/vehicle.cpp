// Copyright 2022 Leonhard S.

#include "vehicle.hpp"

#include <string>

#include "arx/ps2_types.hpp"

namespace ps2 {

int vehicle_from_vehicle_id(arx::vehicle_id_t vehicle_id, Vehicle* vehicle) {
    switch (vehicle_id) {
    case 0:
        *vehicle = Vehicle::None;
        break;
    case 1:
        *vehicle = Vehicle::Flash;
        break;
    case 2:
        *vehicle = Vehicle::Sunderer;
        break;
    case 3:
        *vehicle = Vehicle::Lightning;
        break;
    case 4:
        *vehicle = Vehicle::Magrider;
        break;
    case 5:
        *vehicle = Vehicle::Vanguard;
        break;
    case 6:
        *vehicle = Vehicle::Prowler;
        break;
    case 7:
        *vehicle = Vehicle::Scythe;
        break;
    case 8:
        *vehicle = Vehicle::Reaver;
        break;
    case 9:
        *vehicle = Vehicle::Mosquito;
        break;
    case 10:
        *vehicle = Vehicle::Liberator;
        break;
    case 11:
        *vehicle = Vehicle::Galaxy;
        break;
    case 12:
        *vehicle = Vehicle::Harasser;
        break;
    case 14:
        *vehicle = Vehicle::Valkyrie;
        break;
    case 15:
        *vehicle = Vehicle::Ant;
        break;
    case 100:
        *vehicle = Vehicle::AiTurret;
        break;
    case 150:
        *vehicle = Vehicle::AaTurret;
        break;
    case 151:
        *vehicle = Vehicle::AvTurret;
        break;
    case 2007:
        *vehicle = Vehicle::Colossus;
        break;
    case 2019:
        *vehicle = Vehicle::Bastion;
        break;
    case 2033:
    case 2125:
    case 2129:
        *vehicle = Vehicle::Javelin;
        break;
    case 2122:
    case 2023:
    case 2124:
        *vehicle = Vehicle::Interceptor;
        break;
    case 2136:
        *vehicle = Vehicle::Dervish;
        break;
    default:
        return -1;
    }
    return 0;
}

int vehicle_to_display_name(Vehicle vehicle, std::string* display_name) {
    switch (vehicle) {
    case Vehicle::Flash:
        *display_name = "Flash";
        break;
    case Vehicle::Sunderer:
        *display_name = "Sunderer";
        break;
    case Vehicle::Lightning:
        *display_name = "Lightning";
        break;
    case Vehicle::Magrider:
        *display_name = "Magrider";
        break;
    case Vehicle::Vanguard:
        *display_name = "Vanguard";
        break;
    case Vehicle::Prowler:
        *display_name = "Prowler";
        break;
    case Vehicle::Scythe:
        *display_name = "Scythe";
        break;
    case Vehicle::Reaver:
        *display_name = "Reaver";
        break;
    case Vehicle::Mosquito:
        *display_name = "Mosquito";
        break;
    case Vehicle::Liberator:
        *display_name = "Liberator";
        break;
    case Vehicle::Galaxy:
        *display_name = "Galaxy";
        break;
    case Vehicle::Harasser:
        *display_name = "Harasser";
        break;
    case Vehicle::Valkyrie:
        *display_name = "Valkyrie";
        break;
    case Vehicle::Ant:
        *display_name = "Ant";
        break;
    case Vehicle::AiTurret:
        *display_name = "AI Turret";
        break;
    case Vehicle::AaTurret:
        *display_name = "AA Turret";
        break;
    case Vehicle::AvTurret:
        *display_name = "AV Turret";
        break;
    case Vehicle::Colossus:
        *display_name = "Colossus";
        break;
    case Vehicle::Bastion:
        *display_name = "Bastion";
        break;
    case Vehicle::Javelin:
        *display_name = "Javelin";
        break;
    case Vehicle::Interceptor:
        *display_name = "Interceptor";
        break;
    case Vehicle::Dervish:
        *display_name = "Dervish";
        break;
    default:
        return -1;
    }
    return 0;
}

} // namespace ps2
