// Copyright 2022 Leonhard S.

#ifndef PS2DATA_VEHICLE_HPP
#define PS2DATA_VEHICLE_HPP

#include <string>

#include "id-types.hpp"

namespace ps2
{

    /**
     * Enumeration of vehicles in PlanetSide 2.
     *
     * The "None" member is a special value used to represent that the player
     * is not in a vehicle.
     */
    enum class Vehicle
    {
        None,
        Flash,
        Sunderer,
        Lightning,
        Magrider,
        Vanguard,
        Prowler,
        Scythe,
        Reaver,
        Mosquito,
        Liberator,
        Galaxy,
        Harasser,
        Valkyrie,
        Ant,
        AiTurret,
        AaTurret,
        AvTurret,
        Colossus,
        Bastion,
        Javelin,
        Interceptor,
        Dervish,
    };

    /**
     * Return the vehicle enum value for a given Census API vehicle ID.
     *
     * @param vehicle_id The Census API vehicle ID.
     * @param vehicle The vehicle enum value to be populated.
     * @return 0 on success, -1 on failure.
     */
    int vehicle_from_vehicle_id(VehicleId vehicle_id, Vehicle &vehicle);

    /**
     * Return the display name for a given vehicle enum value.
     *
     * @param vehicle The vehicle enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int vehicle_to_display_name(Vehicle vehicle, std::string &display_name);

} // namespace ps2

#endif // PS2DATA_VEHICLE_HPP
