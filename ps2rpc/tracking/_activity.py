"""Listening component for tracking individual players."""

import asyncio

import auraxium
from auraxium import event, ps2
from PyQt6.QtWidgets import QWidget

from .._qasync import QAsync, signal
from ..game import (GameStateFactory, Ps2Class, Ps2Faction, Ps2Server,
                    Ps2Vehicle, Ps2Zone)


class ActivityTracker(QAsync):
    """Activity tracker for a single in-game character.

    The given character is tracked until :meth:`stop_tracking()` is
    called or until the character logs off. While active, any change in
    the character's game state will trigger the :attr:`state_changed`
    signal.

    The current state is available through the :attr:`current_state`
    attribute.
    """

    _client: auraxium.EventClient
    _factory: GameStateFactory

    def __init__(self, character_id: int, loop: asyncio.AbstractEventLoop,
                 parent: QWidget | None = None) -> None:
        super().__init__(loop=loop, parent=parent)
        self._character_id = character_id

    async def async_setup(self) -> None:
        # Create the Auraxium client
        # TODO: Provide a service ID
        self._client = auraxium.EventClient()
        # Retrieve initial character state and set up the state factory
        await self._initialise_factory()

        # Define event trigger
        self._trigger = event.Trigger(
            event.Death, characters=[self._character_id])
        self._trigger.action = self._on_death
        self._client.add_trigger(self._trigger)

        # Start the client
        loop: asyncio.AbstractEventLoop | None = self._client.loop  # type: ignore
        if loop is None:
            raise RuntimeError('No event loop set for client')
        loop.create_task(self._client.connect())

    async def async_cleanup(self) -> None:
        await self._client.close()

    status_changed = signal()
    tracking_stopped = signal()

    async def _initialise_factory(self) -> None:
        # Get character
        char = await self._client.get_by_id(ps2.Character, self._character_id)
        if char is None:
            # Unable to retrieve character
            # TODO: Log this error
            self.tracking_stopped.emit()
            return
        # Get server
        world = await char.world()
        if world is None:
            # Unable to retrieve server
            # TODO: Log this error
            self.tracking_stopped.emit()
            return
        server = Ps2Server.from_world_id(world.world_id)
        # Get faction
        faction = Ps2Faction.from_faction_id(char.faction_id)
        # Get last class
        profile = Ps2Class.from_profile_id(char.profile_id)
        # Create the state factory
        self._factory = GameStateFactory(
            character_id=self._character_id,
            faction=faction,
            server=server,
            initial_class=profile
        )

    def _on_death(self, evt: event.Death) -> None:
        """Handle a death event."""
        is_killer = evt.attacker_character_id == self._character_id

        # Update the current zone of our tracked character
        zone = Ps2Zone.from_zone_id(evt.zone_id)
        self._factory.set_zone(zone)

        # Update the class or vehicle of our tracked character
        loadout_id = (evt.attacker_loadout_id
                      if is_killer else evt.character_loadout_id)
        profile: Ps2Class | Ps2Vehicle = Ps2Class.from_loadout_id(loadout_id)
        if is_killer and evt.attacker_vehicle_id:
            try:
                profile = Ps2Vehicle(evt.attacker_vehicle_id)
            except ValueError:
                # TODO: Log the fact that we encountered an unknown vehicle ID
                pass
        self._factory.set_profile(profile)

        # Update the team of our tracked character
        # TODO: Uncomment the below once team ID is available in the API
        # team_id = evt.attacker_team_id if is_killer else evt.character_team_id
        # team = Ps2Faction.from_faction_id(team_id)
        # self._factory.set_team(team)
