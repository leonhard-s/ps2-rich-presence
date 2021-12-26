"""Definition of the main application type."""

import asyncio
from typing import List

from PyQt6.QtWidgets import QApplication

from ._listener import Listener
from ._qasync import connect, slot
from ._presence import Status, PresenceStatus

__all__ = [
    'Application',
]


class Application(QApplication):
    """Main PS2RPC application object."""

    _listener: Listener

    def __init__(self, argv: List[str]) -> None:
        super().__init__(argv)

    def start_tracker(self, loop: asyncio.AbstractEventLoop) -> None:
        """Start the event tracker."""
        self._listener = Listener(loop)
        connect(self._listener.status_changed, self.update_status)

    @slot(PresenceStatus)
    def update_status(self, status: PresenceStatus) -> None:
        """Update the status of the client."""
        Status().update(status)
