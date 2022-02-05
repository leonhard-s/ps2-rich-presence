"""Definition of the main application type."""

import asyncio
from typing import List

from PyQt6.QtWidgets import QApplication

from ._rpc import RPCHandler
from .presence import PresenceFactory
from ._qasync import connect, slot
from .tracking import ActivityTracker
from .userstate import UserState
from .ui import MainWindow

__all__ = [
    'Application',
]

# Hard-coded character "Auroram" for testing purposes
_DEBUG_CHAR_ID = 5428072203494645969


class Application(QApplication):
    """Main PS2RPC application object."""

    _listener: ActivityTracker
    _factory: PresenceFactory

    def __init__(self, argv: List[str]) -> None:
        super().__init__(argv)
        self._main_window = MainWindow()
        self._main_window.show()

    def start_tracker(self, loop: asyncio.AbstractEventLoop) -> None:
        """Start the event tracker."""
        self._listener = ActivityTracker(_DEBUG_CHAR_ID, loop)
        connect(self._listener.state_changed, self.update_status)

    @slot()
    def update_status(self) -> None:
        """Update the status of the client."""
        gamestate = self._listener.current_state
        if not hasattr(self, '_factory'):
            self._factory = PresenceFactory(gamestate, UserState.CARROTS)
        # State
        payload = self._factory.build_presence()
        RPCHandler().update(payload)
