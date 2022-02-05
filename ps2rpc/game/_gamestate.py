"""Implementation of the `GameState` class and associated factory."""

import dataclasses

from ._ps2 import Ps2Class, Ps2Faction, Ps2Server, Ps2Vehicle, Ps2Zone

__all__ = [
    'GameState',
    'GameStateFactory',
]


@dataclasses.dataclass(frozen=True)
class GameState:
    """Class representing the current state of a character in PS2."""

    character_id: int
    faction: Ps2Faction
    team: Ps2Faction
    server: Ps2Server
    profile: Ps2Class | Ps2Vehicle
    zone: Ps2Zone


class GameStateFactory:
    """Factory class for generating `GameState` instances.

    The class holds references to immutable parts of the game state
    that cannot be inferred from the event service payloads, such as
    the character's ID, faction, or server.

    Additionally, it also keeps track of the recent state of the
    tracked character so it can be updated incrementally. This is done
    because not every event service payload contains all the data
    necessary to produce a valid game state.
    """

    def __init__(self, character_id: int, faction: Ps2Faction,
                 server: Ps2Server, initial_class: Ps2Class | None = None
                 ) -> None:
        self._character_id = character_id
        self._faction = faction
        self._team = faction
        self._profile: Ps2Class | Ps2Vehicle = (
            initial_class or Ps2Class.LIGHT_ASSAULT)
        self._server = server
        self._zone = Ps2Zone.SANCTUARY

    @property
    def character_id(self) -> int:
        """The character's ID."""
        return self._character_id

    def build_state(self) -> GameState:
        """Build a `GameState` instance from the given data."""
        return GameState(
            character_id=self._character_id,
            faction=self._faction,
            team=self._team,
            server=self._server,
            profile=self._profile,
            zone=self._zone,
        )

    def set_profile(self, profile: Ps2Class | Ps2Vehicle) -> None:
        """Set the profile for the current game state."""
        self._profile = profile

    def set_team(self, team: Ps2Faction) -> None:
        """Set the team for the current game state."""
        self._team = team

    def set_zone(self, zone: Ps2Zone) -> None:
        """Set the zone for the current game state."""
        self._zone = zone
