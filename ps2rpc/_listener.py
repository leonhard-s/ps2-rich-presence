"""Qt-compatible event listener.

This wraps the Auraxium client in a Qt-compatible interface and defines
the signals and slots needed to communicate with the Qt world.
"""

import asyncio
from typing import Optional

import auraxium
from auraxium import event

from ._qasync import QAsync, signal
from ._status import generate_character_status
from ._ps2 import Ps2Class, Ps2Faction, Ps2Vehicle, Ps2Server, Ps2Zone

_DEBUG_CHAR_ID = 5428072203494645969


class Listener(QAsync):
    """Main listener component for the Auraxium event client."""

    _client: auraxium.EventClient

    async def async_setup(self) -> None:
        self._client = auraxium.EventClient()
        # Define event trigger
        self._trigger = event.Trigger(
            event.Death, characters=[_DEBUG_CHAR_ID])
        self._trigger.action = self._on_event
        self._client.add_trigger(self._trigger)

        # Start the client
        loop: Optional[asyncio.AbstractEventLoop] = (
            self._client.loop)  # type: ignore
        if loop is None:
            raise RuntimeError('No event loop set for client')
        loop.create_task(self._client.connect())

    async def async_cleanup(self) -> None:
        await self._client.close()

    status_changed = signal(dict)

    def _on_event(self, evt: event.Death) -> None:
        # Ignore events that aren't for our character
        if (evt.character_id != _DEBUG_CHAR_ID
                and evt.attacker_character_id != _DEBUG_CHAR_ID):
            return
        # Get character
        char_name = 'Auroram'
        # Get faction
        faction = Ps2Faction.VS
        # Get outfit
        outfit_tag = 'URGE'
        # Get class
        loadout_id = 1
        if evt.attacker_character_id == _DEBUG_CHAR_ID:
            loadout_id = evt.attacker_loadout_id
        else:
            loadout_id = evt.character_loadout_id
        profile: Ps2Class | Ps2Vehicle = Ps2Class.from_loadout_id(loadout_id)
        if evt.vehicle_id is not None:
            profile = Ps2Class.from_loadout_id(loadout_id)
        # If the vehicle is known, show that instead
        if (evt.attacker_character_id == _DEBUG_CHAR_ID
                and evt.attacker_vehicle_id is not None):
            try:
                profile = Ps2Vehicle(evt.attacker_vehicle_id)
            except ValueError:
                pass  # keep using class
        # Get zone
        zone = Ps2Zone.from_zone_id(evt.zone_id)
        # Get server
        server = Ps2Server.from_world_id(13)
        # Emit status
        status = generate_character_status(
            char_name, faction, outfit_tag, profile, zone, server)
        self.status_changed.emit(status)
