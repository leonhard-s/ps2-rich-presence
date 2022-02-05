"""This module is reponsible for updating the Discord presence.

The exported :class:`Status` singleton provides hooks for updating the
current presence, and also automatically handles the the Discord RPC
update limit of one update per 15 seconds.
"""

import asyncio
import logging
from typing import Any, Dict, Type

import pypresence  # type: ignore

from .discord import APP_ID
from .presence import PresencePayload

__all__ = [
    'RPCHandler',
]

# Update interval in seconds. This is governed by the Discord RPC guidelines
_UPDATE_INTERVAL = 15.0

_log = logging.getLogger("ps2rpc.rpc")


class _SingletonHandler(type):
    """Metaclass helper for singletons."""

    _instances: Dict[Type[Any], Any] = {}

    def __call__(cls, *args: Any, **kwargs: Any) -> '_SingletonHandler':
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]


class RPCHandler(metaclass=_SingletonHandler):
    """Singleton responsible for maintaining the RPC handshake.

    This class also includes a rate limit to stay within the four
    updates per minute limit, regardless of how often the
    :meth:`update` method is called.

    The :meth:`update` method only updates an internal flag with a
    "set" value, which is compared to the current state when a new
    update is due. If no changes are found, no update is broadcast.
    """

    _status: PresencePayload

    def __init__(self) -> None:
        _log.info('Starting presence RPC client')
        self._rpc = pypresence.AioPresence(APP_ID)
        loop = asyncio.get_event_loop()
        loop.create_task(self._rpc.connect())  # Start handshake loop
        self._last_status: PresencePayload = None  # type: ignore
        loop.create_task(self._loop())  # Start status loop

    def update(self, status: PresencePayload) -> None:
        """Update the RPC status.

        This method will update the internal state cache.
        """
        self._status = status

    async def _check_status(self) -> None:
        """Evaluate the current state cache with the last submitted state."""
        _log.debug('Comparing status cache with last broadcast')
        if (self._status != self._last_status):
            _log.info('Status has changed, broadcasting')
            self._last_status = self._status
            # Strip None fields
            kwargs = {k: v for k, v in self._status.to_dict().items()
                      if v is not None}
            await self._rpc.update(**kwargs)  # type: ignore
        else:
            _log.info('Status has not changed')

    async def _loop(self) -> None:
        """Thread timer loop for the RPC updates; never ends!"""
        while True:
            await asyncio.sleep(_UPDATE_INTERVAL)
            await self._check_status()