"""Game-specific data structures and factories.

This module contains all PlanetSide 2-specific data structures and
converters. Its primary interface are the `GameState` and
`GameStateFactory` classes, which are used to represent the current
state of a character in the game and to update this state from event
service payloads.
"""

from ._gamestate import GameState, GameStateFactory
from ._ps2 import Ps2Class, Ps2Faction, Ps2Vehicle, Ps2Server, Ps2Zone

__all__ = [
    'GameState',
    'GameStateFactory',
    'Ps2Class',
    'Ps2Faction',
    'Ps2Vehicle',
    'Ps2Server',
    'Ps2Zone',
]
