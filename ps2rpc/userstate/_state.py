"""Definition of the state picker GUI and the available state names."""

import enum

__all__ = [
    'UserState',
]


class UserState(enum.Enum):
    """User-selectable statuses for the "state" rich presence field.

    Since this RPC is not integrated into the game's client, we have to
    let the user provide their own status via a GUI dropdown menu.
    """

    # NOTE: This enum defines all displayable states. To add new state strings,
    # simply add them to this enum. The enum value strings will be displayed in
    # the GUI and are also used for the rich presence.

    DEFAULT = ''
    IDLE = 'Idling'
    SOLO = 'Playing Solo'
    ZERG = 'Zerging along'
    # Squad / Team play
    LOOKING_FOR_SQUAD = 'Looking for Squad'
    IN_SQUAD = 'In a Squad'
    OPS = 'Playing Ops'
    # Memery
    CARROTS = 'Looking for Carrots'
    DIRECTIVES = 'Grinding Directives'
    GHOSTCAPPING = 'Ghostcapping'

    def to_string(self) -> str:
        """Return the string version of a given state value."""
        return self.value

    @classmethod
    def get_by_index(cls, index: int) -> 'UserState':
        """Return the :class:`UserState` by index.

        The index will match the definition order of the enum.
        """
        for state_index, state in enumerate(cls):
            if state_index == index:
                return state
        raise IndexError(f'Invalid UserState index: {index}')
