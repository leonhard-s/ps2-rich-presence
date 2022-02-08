// Copyright 2022 Leonhard S.

#ifndef PS2RPC_GAME_STATE_HPP
#define PS2RPC_GAME_STATE_HPP

#include "ps2.hpp"

namespace ps2rpc
{

    /**
     * Mutable representation of the current state of a player in PS2.
     */
    struct GameState
    {
        GameState(ps2::CharacterId character_id,
                  ps2::Faction faction,
                  ps2::Faction team,
                  ps2::Server server,
                  ps2::Class class_,
                  ps2::Vehicle vehicle,
                  ps2::Zone zone);

        ps2::CharacterId character_id;
        ps2::Faction faction;
        ps2::Faction team;
        ps2::Server server;
        ps2::Class class_;
        ps2::Vehicle vehicle;
        ps2::Zone zone;
    };

    /**
     * Factory class for generating a GameState.
     *
     * This class holds the immutable parts of the game state that are boundn
     * to the character and cannot be inferred from event service payloads,
     * such as their ID, faction, or server.
     *
     * Additionally, it keeps track of the recent state of the tracked
     * character to allow incremental updates, since not all service payloads
     * contain all data necessary to generate a valid state.
     */
    class GameStateFactory
    {
    public:
        GameStateFactory(ps2::CharacterId character_id,
                         ps2::Faction faction,
                         ps2::Server server,
                         ps2::Class initial_class = ps2::Class::LightAssault);

        ps2::CharacterId getCharacterId() const noexcept;
        void setProfile(ps2::Class class_) noexcept;
        void setProfile(ps2::Vehicle vehicle) noexcept;
        void setTeam(ps2::Faction team) noexcept;
        void setZone(ps2::Zone zone) noexcept;

        int buildState(GameState &game_state) const;

    private:
        ps2::CharacterId character_id_;
        ps2::Faction faction_;
        ps2::Server server_;
        ps2::Class class_;
        ps2::Vehicle vehicle_;
        ps2::Faction team_;
        ps2::Zone zone_;
    };

} // namespace ps2rpc

#endif // PS2RPC_GAME_STATE_HPP
