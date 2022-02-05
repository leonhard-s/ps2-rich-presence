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
    """

    def __init__(self, character_id: int, faction: Ps2Faction,
                 server: Ps2Server) -> None:
        self._character_id = character_id
        self._faction = faction
        self._server = server

    def build_state(self, team: Ps2Faction, profile: Ps2Class | Ps2Vehicle,
                    zone: Ps2Zone) -> GameState:
        """Build a `GameState` instance from the given data."""
        return GameState(
            character_id=self._character_id,
            faction=self._faction,
            team=team,
            server=self._server,
            profile=profile,
            zone=zone,
        )
