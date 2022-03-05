// Copyright 2022 Leonhard S.

#ifndef PS2DATA_FACTION_HPP
#define PS2DATA_FACTION_HPP

#include <string>

#include "id-types.hpp"

namespace ps2
{

    /** Enumeration of factions in PlanetSide 2. */
    enum class Faction
    {
        NS,
        NC,
        TR,
        VS,
        NSO,
    };

    /**
     * Return the faction enum value for a given Census API faction ID.
     *
     * @param faction_id The Census API faction ID.
     * @param faction The faction enum value to be populated.
     * @return 0 on success, -1 on failure.
     */
    int faction_from_faction_id(FactionId faction_id, Faction &faction);

    /**
     * Return the display name for a given faction enum value.
     *
     * @param faction The faction enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int faction_to_display_name(Faction faction, std::string &display_name);

    /**
     * Return the faction tag for a given faction enum value.
     *
     * @param faction The faction enum value.
     * @param tag The faction tag to be populated.
     * @return 0 on success, -1 on failure.
     */
    int faction_to_tag(Faction faction, std::string &tag);

} // namespace ps2

#endif // PS2DATA_FACTION_HPP