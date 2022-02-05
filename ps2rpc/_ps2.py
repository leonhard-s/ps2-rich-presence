"""Game-specific enums and converter functions.

This module contains everything that could break with major game
updates, with a bonus sprinkle of hard-coded ID values.
"""

import enum

__all__ = [
    'Ps2Class',
    'Ps2Faction',
    'Ps2Zone',
    'Ps2Server',
]

_CLASS_LOADOUTS = {
    # Infiltrator
    1: 1,  # NC
    8: 1,  # TR
    15: 1,  # VS
    28: 1,  # NSO

    # Light Assault
    2: 2,  # NC
    10: 2,  # TR
    17: 2,  # VS
    29: 2,  # NSO

    # Combat Medic
    4: 3,  # NC
    11: 3,  # TR
    18: 3,  # VS
    30: 3,  # NSO

    # Engineer
    5: 4,  # NC
    12: 4,  # TR
    19: 4,  # VS
    31: 4,  # NSO

    # Heavy Assault
    6: 5,  # NC
    13: 5,  # TR
    20: 5,  # VS
    32: 5,  # NSO

    # MAX
    7: 6,  # NC
    14: 6,  # TR
    21: 6,  # VS
    45: 6,  # NSO
}

_CLASS_PROFILES = {
    # Infiltrator
    2: 1,  # NC
    10: 1,  # TR
    17: 1,  # VS
    190: 1,  # NSO

    # Light Assault
    4: 2,  # NC
    12: 2,  # TR
    19: 2,  # VS
    191: 2,  # NSO

    # Combat Medic
    5: 3,  # NC
    14: 3,  # TR
    20: 3,  # VS
    192: 3,  # NSO

    # Engineer
    6: 4,  # NC
    14: 4,  # TR
    21: 4,  # VS
    193: 4,  # NSO

    # Heavy Assault
    7: 5,  # NC
    15: 5,  # TR
    22: 5,  # VS
    194: 5,  # NSO

    # MAX
    8: 6,  # NC
    16: 6,  # TR
    23: 6,  # VS
    252: 6,  # NSO
}

_PLAYABLE_FACTIONS = {
    1: 1,  # VS
    2: 2,  # NC
    3: 4,  # TR
    4: 4,  # NSO
}

_FACTION_NAMES = {
    0: 'Nanite Systems',
    1: 'Vanu Sovereignty',
    2: 'New Conglomerate',
    3: 'Terran Republic',
    4: 'Nanite Systems Operators',
}

_FACTION_TAGS = {
    0: '',  # No faction
    1: 'VS',  # Vanu Sovereignty
    2: 'NC',  # New Conglomerate
    3: 'TR',  # Terran Republic
    4: 'NSO',  # Nanite Systems Operators
}

_STATIC_ZONES = {
    2: 1,  # Indar
    4: 2,  # Hossin
    6: 3,  # Amerish
    8: 4,  # Esamir
}

_GAME_SERVERS = {
    1: 1,  # Connery
    10: 2,  # Miller
    13: 3,  # Cobalt
    17: 4,  # Emerald
    19: 5,  # Jaeger
    40: 6,  # SolTech
}


class Ps2Class(enum.Enum):
    """An infentry class in the game."""

    INFILTRATOR = 1
    LIGHT_ASSAULT = 2
    COMBAT_MEDIC = 3
    ENGINEER = 4
    HEAVY_ASSAULT = 5
    MAX = 6

    def display_name(self) -> str:
        """Return the class's display name."""
        if self.name == 'MAX':
            return self.name
        return self.name.replace('_', ' ').title()

    @classmethod
    def from_loadout_id(cls, loadout_id: int) -> 'Ps2Class':
        """Convert a loadout ID to a class."""
        return cls(_CLASS_LOADOUTS.get(loadout_id, 2))  # LA by default

    @classmethod
    def from_profile_id(cls, profile_id: int) -> 'Ps2Class':
        """Convert a profile ID to a class."""
        return cls(_CLASS_PROFILES.get(profile_id, 2))  # LA by default


class Ps2Faction(enum.Enum):
    """A playable faction in the game."""

    VS = 1
    NC = 2
    TR = 3
    NSO = 4

    @classmethod
    def from_faction_id(cls, faction_id: int) -> 'Ps2Faction':
        """Convert a faction ID to a faction."""
        return cls(_PLAYABLE_FACTIONS.get(faction_id, 4))  # NSO by default

    def tag(self) -> str:
        """Return the faction's tag."""
        return _FACTION_TAGS.get(self.value, '')

    def display_name(self) -> str:
        """Return a faction's display name."""
        return _FACTION_NAMES.get(self.value, 'Nanite Systems')


class Ps2Vehicle(enum.Enum):
    """A vehicle in the game."""

    FLASH = 1
    SUNDERER = 2
    LIGHTNING = 3
    MAGRIDER = 4
    VANGUARD = 5
    PROWLER = 6
    SCYTHE = 7
    REAVER = 8
    MOSQUITO = 9
    LIBERATOR = 10
    GALAXY = 11
    HARASSER = 12
    VALKYRIE = 14
    ANT = 15
    AI_TURRET = 100
    AA_TURRET = 150
    AV_TURRET = 151
    COLOSSUS = 2007
    BASTION = 2019
    JAVELIN = 2033
    INTERCEPTOR_TR = 2122
    INTERCEPTOR_NC = 2023
    INTERCEPTOR_VS = 2124
    JAVELIN_ = 2125
    JAVELIN__ = 2129
    DERVISH = 2136

    def display_name(self) -> str:
        """Return a vehicle's display name."""
        if self.name.lower().startswith('INTERCEPTOR'):
            return f'Interceptor'
        return self.name.replace('_', ' ').rstrip().title()


class Ps2Zone(enum.Enum):
    """A zone in the game."""

    # Default zone
    SANCTUARY = 0

    # Continents
    INDAR = 1
    HOSSIN = 2
    AMERISH = 3
    ESAMIR = 4
    # OSHUR = 5

    # Outfit Wars
    DESOLATION = 6

    # Special zones
    TUTORIAL = 7
    VR_TRAINING = 8

    @classmethod
    def default(cls) -> 'Ps2Zone':
        """Return the default zone, used upon login and on error."""
        return cls.SANCTUARY

    def display_name(self) -> str:
        """Return a zone's display name."""
        if self == Ps2Zone.VR_TRAINING:
            return 'VR Training'
        return self.name.title()

    @classmethod
    def from_zone_id(cls, zone_id: int) -> 'Ps2Zone':
        """Convert a zone ID to a :class:`Ps2Zone`.

        This method allows for handling of dynamic zone IDs and is the
        only way this class may be instantiated.
        """
        # TODO: This is hacky and for testing, proper handling of dynamic zone
        # IDs requries more thought than I am willing to invest today.
        return cls(_STATIC_ZONES.get(zone_id, 0))  # Sanctuary by default


class Ps2Server(enum.Enum):
    """A physical server for the game."""

    CONNERY = 1  # Connery
    MILLER = 2  # Miller
    COBALT = 3  # Cobalt
    EMERALD = 4  # Emerald
    JAEGER = 5  # Jaeger
    SOLTECH = 6  # SolTech

    @classmethod
    def from_world_id(cls, world_id: int) -> 'Ps2Server':
        """Convert a world ID to a :class:`Ps2Server`."""
        return cls(_GAME_SERVERS.get(world_id, 5))  # Jaeger by default
