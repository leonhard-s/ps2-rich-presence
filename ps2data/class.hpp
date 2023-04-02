// Copyright 2022 Leonhard S.

#pragma once

#include <string>

#include "arx/ps2_types.hpp"

#include "faction.hpp"

namespace ps2
{

    /** Enumeration of infantry classes in PlanetSide 2. */
    enum class Class
    {
        Infiltrator,
        LightAssault,
        CombatMedic,
        Engineer,
        HeavyAssault,
        MAX,
    };

    /**
     * Return the class enum value for a given Census API loadout ID.
     *
     * @param loadout_id The Census API loadout ID.
     * @param class_ The class enum value to be populated.
     * @return 0 on success, -1 on failure.
     */
    int class_from_loadout_id(arx::loadout_id_t loadout_id, Class &class_);

    /**
     * Return the class enum value for a given Census API profile ID.
     *
     * @param profile_id The Census API profile ID.
     * @param class_ The class enum value to be populated.
     * @return 0 on success, -1 on failure.
     */
    int class_from_profile_id(arx::profile_id_t profile_id, Class &class_);

    /**
     * Return the display name for a given class enum value.
     *
     * @param class_ The class enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int class_to_display_name(Class class_, std::string &display_name);

    /**
     * Return the Census API profile ID for a given class enum value.
     *
     * @param class_ The class enum value.
     * @param faction The faction of the profile to return.
     * @return The Census API profile ID.
     */
    arx::profile_id_t class_to_profile_id(Class class_, Faction faction);

} // namespace ps2
