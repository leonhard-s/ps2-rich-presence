"""This module is reponsible for updating the Discord presence.

The exported :class:`Status` singleton provides hooks for updating the
current presence, and also automatically handles the the Discord RPC
update limit of one update per 15 seconds.
"""

import logging
import threading
from typing import Any, Dict, Optional, Type, TypedDict

import pypresence  # type: ignore

# Client ID of the Discord application to use.
_CLIENT_ID = 923122197793488997
# Update interval in seconds. This is governed by the Discord RPC guidelines
_UPDATE_INTERVAL = 15.0

_log = logging.getLogger("ps2rpc.presence")


class PresenceStatus(TypedDict):
    """A dict-like representation of the data to be sent to Discord.

    state: current playing status, e.g. "Looking to play", "In a game"
    details: current game details, e.g. "Competitive", "In Queue"
    start_time: epoch seconds since start, will show time as "elapsed"
    end_time: epoch seconds till end, will show time as "remaining"
    large_image: name of the image asset to display
    large_text: text to display on the large image
    small_image: name of the image asset to display
    small_text: text to display on the small image
    """

    state: Optional[str]
    details: Optional[str]
    start: Optional[int]
    end: Optional[int]
    large_image: Optional[str]
    large_text: Optional[str]
    small_image: Optional[str]
    small_text: Optional[str]


class _SingletonHandler(type):
    """Metaclass helper for singletons."""

    _instances: Dict[Type[Any], Any] = {}

    def __call__(cls, *args: Any, **kwargs: Any) -> '_SingletonHandler':
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]


class Status(metaclass=_SingletonHandler):
    """Singleton responsible for maintaining the RPC handshake.

    This class also includes a rate limit to stay within the four
    updates per minute limit, regardless of how often the
    :meth:`update` method is called.

    The :meth:`update` method only updates an internal flag with a
    "set" value, which is compared to the current state when a new
    update is due. If no changes are found, no update is broadcast.
    """

    def __init__(self, status: PresenceStatus) -> None:
        _log.info('Starting presence RPC client')
        self._rpc = pypresence.Presence(_CLIENT_ID)
        self._rpc.connect()  # Start handshake loop
        self._timer = threading.Timer(_UPDATE_INTERVAL, self._loop)

        self._status: PresenceStatus = status
        self._last_status: PresenceStatus = {}  # type: ignore

    def update(self, status: PresenceStatus) -> None:
        """Update the RPC status.

        This method will update the internal state cache.
        """
        self._status = status

    def start(self) -> None:
        """Start the timer loop.

        This will immediately update the RPC status once, as well as
        start the timer loop. This method returns immediately; the
        timer lives in a separate thread.
        """
        self._loop()
        try:
            self._timer.start()
        except RuntimeError:
            _log.info('Timer already started')
        else:
            _log.info('Timer thread started')

    def stop(self) -> None:
        """Stop the external thread timer."""
        self._timer.cancel()
        _log.info('Timer thread cancelled')

    def _check_status(self) -> None:
        """Evaluate the current state cache with the last submitted state."""
        _log.info('Comparing status cache with last broadcast')
        if (self._status != self._last_status):
            _log.info('Status has changed, broadcasting')
            self._last_status = self._status
            # Strip None fields
            kwargs = {k: v for k, v in self._status.items() if v is not None}
            self._rpc.update(**kwargs)  # type: ignore
        else:
            _log.info('Status has not changed')

    def _loop(self) -> None:
        """Thread timer loop for the RPC updates; never ends!"""
        self._check_status()
        self._timer = threading.Timer(_UPDATE_INTERVAL, self._loop)
        self._timer.start()
