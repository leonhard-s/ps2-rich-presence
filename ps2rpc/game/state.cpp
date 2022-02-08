// Copyright 2022 Leonhard S.

#include "game/state.hpp"

#include "ps2.hpp"

namespace ps2rpc
{

    GameState::GameState(ps2::CharacterId character_id, ps2::Faction faction, ps2::Faction team, ps2::Server server, ps2::Class class_, ps2::Vehicle vehicle, ps2::Zone zone)
        : character_id{character_id}, faction{faction}, team{team}, server{server}, class_{class_}, vehicle{vehicle}, zone{zone} {}

    GameStateFactory::GameStateFactory(ps2::CharacterId character_id, ps2::Faction faction, ps2::Server server, ps2::Class class_)
        : character_id_{character_id}, faction_{faction}, server_{server}, class_{class_}, vehicle_{ps2::Vehicle::Flash}, team_{ps2::Faction::NS}, zone_{ps2::Zone::Sanctuary} {}

    ps2::CharacterId GameStateFactory::getCharacterId() const noexcept
    {
        return character_id_;
    }

    void GameStateFactory::setProfile(ps2::Class class_) noexcept
    {
        this->class_ = class_;
    }

    void GameStateFactory::setProfile(ps2::Vehicle vehicle) noexcept
    {
        vehicle_ = vehicle;
    }

    void GameStateFactory::setTeam(ps2::Faction team) noexcept
    {
        team_ = team;
    }

    void GameStateFactory::setZone(ps2::Zone zone) noexcept
    {
        zone_ = zone;
    }

    int GameStateFactory::buildState(GameState &game_state) const
    {
        game_state = GameState{character_id_, faction_, team_, server_, class_, vehicle_, zone_};
        return 0;
    }

} // namespace ps2rpc
