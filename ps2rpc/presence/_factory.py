"""Main presence factory.

This class is used to generate the presence data for the Discord
application. It is a mutable factory responsible for combining the
user and game states into a single presence object.
"""

from ..game import GameState, Ps2Class
from ..userstate import UserState
from ._payload import PresencePayload

__all__ = [
    'PresenceFactory',
]


class PresenceFactory:
    """Factory class for creating Discord presence payloads.

    After initialisation, use the :meth:`update_user_state` and
    :meth:`update_game_state` methods to update the presence data. Run
    :meth:`build_presence` to generate the payload.
    """

    def __init__(self, game_state: GameState,
                 user_state: UserState = UserState.DEFAULT) -> None:
        self._user_state = user_state
        self._game_state = game_state

    def build_presence(self) -> PresencePayload:
        """Build a Discord presence payload."""
        large_image, large_text = self._get_large_image()
        small_image, small_text = self._get_small_image()
        return PresencePayload(
            state=self._get_state(),
            details=self._get_details(),
            start=None,
            end=None,
            large_image=large_image,
            large_text=large_text,
            small_image=small_image,
            small_text=small_text,
        )

    def update_user_state(self, user_state: UserState) -> None:
        """Update the current user state.

        This is called in response to the user changing their state in
        the GUI.
        """
        self._user_state = user_state

    def update_game_state(self, game_state: GameState) -> None:
        """Update the current game state.

        This is called regularly in response to in-game events.
        """
        self._game_state = game_state

    def _get_large_image(self) -> tuple[str, str]:
        """Generate the large image key and text."""
        zone = self._game_state.zone
        return zone.name.lower(), zone.display_name()

    def _get_small_image(self) -> tuple[str, str]:
        """Generate the small image key and text."""
        profile = self._game_state.profile
        # Customise the image text depending on the class/vehicle
        if isinstance(profile, Ps2Class):
            text = f'Last seen playing {profile.display_name()}'
        else:
            # Ps2Vehicle
            vehicle = profile.display_name()
            text = 'Last seen in a'
            if vehicle.lower().startswith(('a', 'e', 'i', 'o', 'u')):
                text += 'n'
            text += f' {vehicle}'
        return profile.name.lower(), text

    def _get_state(self) -> str | None:
        """Generate the "state" string for the presence."""
        state: str | None = str(self._user_state.value)
        # NOTE: The state string may not be empty; must be set to None instead
        if not state:
            state = None
        return state

    def _get_details(self) -> str:
        """Generate the "details" string for the presence."""
        faction = self._game_state.faction
        team = self._game_state.team
        # Start with full faction name
        details = faction.display_name()
        # Append the team tag for NSOs
        if faction != team:
            details += f' on {team.tag()}'
        # Append the server name
        server_name = self._game_state.server.display_name()
        details += f' ({server_name})'
        return details
