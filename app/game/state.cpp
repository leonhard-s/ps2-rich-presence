// Copyright 2022 Leonhard S.

#include "game/state.hpp"

#include "ps2.hpp"

namespace PresenceApp {

GameState::GameState()
    : character_id_{ 0 }
    , faction_{ 0 }
    , team_{ 0 }
    , server_{ 0 }
    , class_{ 0 }
    , vehicle_{ 0 }
    , zone_{ 0 } {}

GameState::GameState(
    arx::character_id_t character_id,
    ps2::Faction faction,
    ps2::Faction team,
    ps2::Server server,
    ps2::Class cls,
    ps2::Vehicle vehicle,
    ps2::Zone zone
)
    : character_id_{ character_id }
    , faction_{ faction }
    , team_{ team }
    , server_{ server }
    , class_{ cls }
    , vehicle_{ vehicle }
    , zone_{ zone } {}

bool GameState::operator==(const GameState& other) const {
    return character_id_ == other.character_id_ &&
        faction_ == other.faction_ &&
        team_ == other.team_ &&
        server_ == other.server_ &&
        class_ == other.class_ &&
        vehicle_ == other.vehicle_ &&
        zone_ == other.zone_;
}

bool GameState::operator!=(const GameState& other) const {
    return !(*this == other);
}

GameStateFactory::GameStateFactory(
    arx::character_id_t character_id,
    ps2::Faction faction,
    ps2::Server server,
    ps2::Class cls
)
    : character_id_{ character_id }
    , faction_{ faction }
    , server_{ server }
    , class_{ cls }
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

void GameStateFactory::setProfile(ps2::Class cls) noexcept {
    this->class_ = cls;
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

} // namespace PresenceApp
