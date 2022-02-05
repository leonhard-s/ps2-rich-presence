// Copyright 2022 Leonhard S.

#ifndef PS2RPC_PS2_HPP
#define PS2RPC_PS2_HPP

#include <cstdint>
#include <string>

namespace ps2
{

    /** Big S unsigned integer type for representing character IDs. */
    using CharacterId = int_least64_t;

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
    int class_from_loadout_id(int loadout_id, Class &class_);
    /**
     * Return the class enum value for a given Census API profile ID.
     *
     * @param profile_id The Census API profile ID.
     * @param class_ The class enum value to be populated.
     * @return 0 on success, -1 on failure.
     */
    int class_from_profile_id(int profile_id, Class &class_);
    /**
     * Return the display name for a given class enum value.
     *
     * @param class_ The class enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int class_to_display_name(Class class_, std::string &display_name);

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
    int faction_from_faction_id(int faction_id, Faction &faction);
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
    int vehicle_from_vehicle_id(int vehicle_id, Vehicle &vehicle);
    /**
     * Return the display name for a given vehicle enum value.
     *
     * @param vehicle The vehicle enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int vehicle_to_display_name(Vehicle vehicle, std::string &display_name);

    /** Enumeration of game servers in PlanetSide 2. */
    enum class Server
    {
        Cobalt,
        Connery,
        Emerald,
        Jaeger,
        Miller,
        SolTech,
    };
    /**
     * Return the server enum value for a given Census API world ID.
     *
     * @param world_id The Census API world ID.
     * @param server The server enum value to be populated.
     * @return 0 on success, -1 on failure.
     */
    int server_from_world_id(int world_id, Server &server);
    /**
     * Return the display name for a given server enum value.
     *
     * @param server The server enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int server_to_display_name(Server server, std::string &display_name);

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
    int zone_from_zone_id(int zone_id, Zone &zone);
    /**
     * Return the display name for a given zone enum value.
     *
     * @param zone The zone enum value.
     * @param display_name The display name to be populated.
     * @return 0 on success, -1 on failure.
     */
    int zone_to_display_name(Zone zone, std::string &display_name);

} // namespace

#endif // PS2RPC_PS2_HPP
