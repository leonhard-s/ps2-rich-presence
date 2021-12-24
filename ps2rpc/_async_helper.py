"""Implementation of the AsyncWrapper helper class."""

import asyncio
import logging
from typing import Callable, Optional

from PyQt6.QtCore import QCoreApplication, QObject
from PyQt6.QtWidgets import QApplication, QWidget

from ._typing_utils import asyncSlot, connect, QEventLoop

__all__ = [
    'AsyncHelper',
    'get_qasync_event_loop',
]

_log = logging.getLogger('async_helper')


class AsyncHelper(QObject):
    """A helper object for maintaining the asyncio event loop in Qt.

    This class can be subclassed by other objects to wrap their async
    components in neat Qt-style classes.
    """

    def __init__(self, loop: asyncio.AbstractEventLoop,
                 parent: Optional[QWidget] = None) -> None:
        super().__init__(parent=parent)
        # Configure the event loop
        self._loop = loop or asyncio.get_event_loop()
        if not isinstance(self._loop, QEventLoop):
            raise RuntimeError(
                'Invalid asyncio loop type, QEventLoop required.')
        # Schedule setup hook
        # TODO: Figure out if there is a reason to do this through a slot
        # rather than a "raw" asyncio task.
        self._loop.create_task(self.async_setup())
        # Schedule cleanup hook
        connect(self.destroyed, self._slot_async_cleanup)

    @asyncSlot
    async def _slot_async_cleanup(self) -> None:
        """Slot forwarding the async_cleanup hook."""
        await self.async_cleanup()

    async def async_setup(self) -> None:
        """Async setup hook.

        Use this to perform asynchronous initialisation of your async
        classes. It will be run automatically as soon as possible
        after initialisation.

        The default implementation does nothing.
        """

    async def async_cleanup(self) -> None:
        """Async cleanup hook.

        Use this to perform asynchronous cleanup of your async classes.
        It will be run automatically just before destruction of the
        object.

        The default implementation does nothing.
        """


def _get_qasync_event_loop() -> Callable[[], asyncio.AbstractEventLoop]:
    """Retrieve the global QEventLoop for asyncio.

    Will definitely break for Python-level multithreading - but so will
    Qt, so what gives.
    """
    # Nonlocals for scope-based pseudo-singleton
    _loop: asyncio.AbstractEventLoop
    _has_loop: bool = False

    def _inner() -> asyncio.AbstractEventLoop:
        nonlocal _loop, _has_loop
        # Ensure a QApplication exists
        app: Optional[QCoreApplication] = QApplication.instance()
        if app is None:
            raise RuntimeError(
                'Unable to create QEventLoop without a running QApplication.')
        # Create the loop if none exists yet
        if not _has_loop:
            _loop = QEventLoop()
            asyncio.set_event_loop(_loop)
            _has_loop = True
        # Return the loop
        return _loop

    return _inner


get_qasync_event_loop = _get_qasync_event_loop()
