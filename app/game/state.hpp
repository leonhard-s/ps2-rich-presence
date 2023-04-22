// Copyright 2022 Leonhard S.

#pragma once

#include "ps2.hpp"

namespace PresenceApp {

/**
 * Mutable representation of the current state of a player in PS2.
 */
struct GameState {
    GameState();
    GameState(
        arx::character_id_t character_id,
        ps2::Faction faction,
        ps2::Faction team,
        ps2::Server server,
        ps2::Class cls,
        ps2::Vehicle vehicle,
        ps2::Zone zone);

    bool operator==(const GameState& other) const;
    bool operator!=(const GameState& other) const;

    arx::character_id_t character_id_;
    ps2::Faction faction_;
    ps2::Faction team_;
    ps2::Server server_;
    ps2::Class class_;
    ps2::Vehicle vehicle_;
    ps2::Zone zone_;
};

/**
 * Factory class for generating a GameState.
 *
 * This class holds the immutable parts of the game state that are bound
 * to the character and cannot be inferred from event service payloads,
 * such as their ID, faction, or server.
 *
 * Additionally, it keeps track of the recent state of the tracked
 * character to allow incremental updates, since not all service payloads
 * contain all data necessary to generate a valid state.
 */
class GameStateFactory {
public:
    GameStateFactory(
        arx::character_id_t character_id,
        ps2::Faction faction,
        ps2::Server server,
        ps2::Class initial_class = ps2::Class::LightAssault);

    arx::character_id_t getCharacterId() const noexcept;
    ps2::Faction getFaction() const noexcept;
    void setFaction(ps2::Faction faction) noexcept;
    ps2::Server getServer() const noexcept;
    void setServer(ps2::Server server) noexcept;
    ps2::Class getProfileAsClass() const noexcept;
    ps2::Vehicle getProfileAsVehicle() const noexcept;
    void setProfile(ps2::Class cls) noexcept;
    void setProfile(ps2::Vehicle vehicle) noexcept;
    ps2::Faction getTeam() const noexcept;
    void setTeam(ps2::Faction team) noexcept;
    ps2::Zone getZone() const noexcept;
    void setZone(ps2::Zone zone) noexcept;

    int buildState(GameState* game_state) const;

private:
    arx::character_id_t character_id_;
    ps2::Faction faction_;
    ps2::Server server_;
    ps2::Class class_;
    ps2::Vehicle vehicle_;
    ps2::Faction team_;
    ps2::Zone zone_;
};

} // namespace PresenceApp
