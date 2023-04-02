// Copyright 2022 Leonhard S.

#include "game/state.hpp"

#include "ps2.hpp"

namespace ps2rpc {

GameState::GameState()
    : character_id{ 0 }
    , faction{ 0 }
    , team{ 0 }
    , server{ 0 }
    , class_{ 0 }
    , vehicle{ 0 }
    , zone{ 0 } {}

GameState::GameState(
    arx::character_id_t character_id,
    ps2::Faction faction,
    ps2::Faction team,
    ps2::Server server,
    ps2::Class class_,
    ps2::Vehicle vehicle,
    ps2::Zone zone
)
    : character_id{ character_id }
    , faction{ faction }
    , team{ team }
    , server{ server }
    , class_{ class_ }
    , vehicle{ vehicle }
    , zone{ zone } {}

bool GameState::operator==(const GameState& other) const {
    return character_id == other.character_id &&
        faction == other.faction &&
        team == other.team &&
        server == other.server &&
        class_ == other.class_ &&
        vehicle == other.vehicle &&
        zone == other.zone;
}

bool GameState::operator!=(const GameState& other) const {
    return !(*this == other);
}

GameStateFactory::GameStateFactory(
    arx::character_id_t character_id,
    ps2::Faction faction,
    ps2::Server server,
    ps2::Class class_
)
    : character_id_{ character_id }
    , faction_{ faction }
    , server_{ server }
    , class_{ class_ }
    , vehicle_{ ps2::Vehicle::None }
    , team_{ faction }
    , zone_{ ps2::Zone::Sanctuary } {}

arx::character_id_t GameStateFactory::getCharacterId() const noexcept {
    return character_id_;
}

ps2::Faction GameStateFactory::getFaction() const noexcept {
    return faction_;
}

void GameStateFactory::setFaction(ps2::Faction faction) noexcept {
    faction_ = faction;
}

ps2::Server GameStateFactory::getServer() const noexcept {
    return server_;
}

void GameStateFactory::setServer(ps2::Server server) noexcept {
    server_ = server;
}

ps2::Class GameStateFactory::getProfileAsClass() const noexcept {
    return class_;
}

ps2::Vehicle GameStateFactory::getProfileAsVehicle() const noexcept {
    return vehicle_;
}

void GameStateFactory::setProfile(ps2::Class klass) noexcept {
    this->class_ = klass;
    vehicle_ = ps2::Vehicle::None;
}

void GameStateFactory::setProfile(ps2::Vehicle vehicle) noexcept {
    vehicle_ = vehicle;
}

ps2::Faction GameStateFactory::getTeam() const noexcept {
    return team_;
}

void GameStateFactory::setTeam(ps2::Faction team) noexcept {
    team_ = team;
}

ps2::Zone GameStateFactory::getZone() const noexcept {
    return zone_;
}

void GameStateFactory::setZone(ps2::Zone zone) noexcept {
    zone_ = zone;
}

int GameStateFactory::buildState(GameState* game_state) const {
    *game_state = GameState{
        character_id_, faction_, team_, server_, class_, vehicle_, zone_ };
    return 0;
}

} // namespace ps2rpc
