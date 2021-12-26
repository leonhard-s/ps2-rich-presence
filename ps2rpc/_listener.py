"""Qt-compatible event listener.

This wraps the Auraxium client in a Qt-compatible interface and defines
the signals and slots needed to communicate with the Qt world.
"""

import asyncio
from typing import Optional, Union

import auraxium
from auraxium import event, ps2

from ._qasync import QAsync

_SERVICE_ID = 's:example'


class Listener(QAsync):
    """Main listener component for the Auraxium event client."""

    _client: auraxium.EventClient

    async def async_setup(self) -> None:
        self._client = auraxium.EventClient(service_id=_SERVICE_ID)

        @self._client.trigger(event.PlayerLogin, event.PlayerLogout)
        async def print_logins(
                evt: Union[event.PlayerLogin, event.PlayerLogout]) -> None:
            # Get character
            char = await self._client.get_by_id(ps2.Character, evt.character_id)
            if char is None:
                return
            # Get server
            world = await char.world()
            if world is None:
                return
            server = world.name.en if world.name else None
            if server is None:
                return
            # Print name
            name = await char.name_long()
            if isinstance(evt, event.PlayerLogin):
                print('Login: ', name, f'({server})')
            else:
                print('Logout:', name, f'({server})')

        # Silence complaints about unused variables
        _ = print_logins

        # Start the client
        loop: Optional[asyncio.AbstractEventLoop] = (
            self._client.loop)  # type: ignore
        if loop is None:
            raise RuntimeError('No event loop set for client')
        loop.create_task(self._client.connect())

    async def async_cleanup(self) -> None:
        print('Closing client...')
        await self._client.close()
