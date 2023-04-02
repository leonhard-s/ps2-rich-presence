// Copyright 2022 Leonhard S.

#pragma once

#include <string>

#include "arx/ps2_types.hpp"

namespace ps2
{

    /** Enumeration of playable zones in PlanetSide 2. */
    enum class Zone
    {
        Indar,
        Hossin,
        Amerish,
        Esamir,
        Koltyr,
        VrTraining,
        Oshur,
        Desolation,
        Sanctuary,
        Tutorial,
    };
    /**
     * Return the zone enum value for a given Census API zone ID.
     *
     * @param zone_id The Census API zone ID.
     * @param zone The zone enum value to be populated.
     * @return 0 on success, -1 on failure.
     */
    int zone_from_zone_id(arx::zone_id_t zone_id, Zone &zone);
    /**
     * Return the display name for a given zone enum value.
     *
     * @param zone The zone enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int zone_to_display_name(Zone zone, std::string &display_name);

} // namespace ps2
