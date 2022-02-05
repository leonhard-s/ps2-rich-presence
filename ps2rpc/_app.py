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


class Application(QApplication):
    """Main PS2RPC application object."""

    _listener: ActivityTracker
    _factory: PresenceFactory
    _loop: asyncio.AbstractEventLoop

    def __init__(self, argv: List[str]) -> None:
        super().__init__(argv)
        self._main_window = MainWindow()
        connect(self._main_window.character_tracked, self._start_tracking)
        connect(self._main_window.character_untracked, self._stop_tracking)

    def run(self, loop: asyncio.AbstractEventLoop) -> None:
        self._loop = loop
        self._main_window.show()

    @slot(str)
    def _start_tracking(self, character_id: str) -> None:
        self._listener = ActivityTracker(int(character_id), self._loop)
        connect(self._listener.state_changed, self.update_status)
        print(f'Tracker started for {character_id}')

    @slot(str)
    def _stop_tracking(self, character_id: str) -> None:
        # TODO: Check if we need to manually disconenct the listener
        del self._listener

    @slot()
    def update_status(self) -> None:
        """Update the status of the client."""
        gamestate = self._listener.current_state
        if not hasattr(self, '_factory'):
            self._factory = PresenceFactory(gamestate, UserState.CARROTS)
        # State
        payload = self._factory.build_presence()
        RPCHandler().update(payload)
