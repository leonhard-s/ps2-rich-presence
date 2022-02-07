// Copyright 2022 Leonhard S.

#ifndef PS2DATA_CLASS_HPP
#define PS2DATA_CLASS_HPP

#include <string>

#include "id-types.hpp"

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
    int class_from_loadout_id(LoadoutId loadout_id, Class &class_);

    /**
     * Return the class enum value for a given Census API profile ID.
     *
     * @param profile_id The Census API profile ID.
     * @param class_ The class enum value to be populated.
     * @return 0 on success, -1 on failure.
     */
    int class_from_profile_id(ProfileId profile_id, Class &class_);

    /**
     * Return the display name for a given class enum value.
     *
     * @param class_ The class enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int class_to_display_name(Class class_, std::string &display_name);

} // namespace ps2

#endif // PS2DATA_CLASS_HPP
