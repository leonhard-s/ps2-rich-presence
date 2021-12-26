"""Utilities and types for working with asyncio in Qt."""

import asyncio
import sys
from typing import Any, Callable, Optional, ParamSpec, Type, TypeVar

from qasync import (asyncSlot as _asyncSlot, QEventLoop as _QEventLoop,
                    DefaultQEventLoopPolicy as QEventLoopPolicy)
from PyQt6.QtCore import (QCoreApplication, QObject, pyqtSignal as _pyqtSignal,
                          pyqtSlot as _pyqtSlot)
from PyQt6.QtWidgets import QApplication, QWidget
from pypresence import utils as pypresence_utils  # type: ignore

__all__ = [
    'QAsync',
    'QEventLoop',
    'QEventLoopPolicy',
    'async_slot',
    'connect',
    'get_qasync_event_loop',
    'signal',
    'slot',
]

_T = TypeVar('_T')
_P = ParamSpec('_P')


class _EventLoopLike(asyncio.AbstractEventLoop):
    """A dummy class mimicking the QEventLoop interface.

    This class does nothing and is only as a type hint for the
    `QEventLoop` type.
    """

    def __init__(self, *args: Any, **kwargs: Any) -> None:
        raise NotImplementedError()

    def __call__(self, app: QApplication) -> '_EventLoopLike':
        raise NotImplementedError()

    def __enter__(self) -> asyncio.AbstractEventLoop:
        raise NotImplementedError()

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> bool:
        raise NotImplementedError()


QEventLoop: Type[_EventLoopLike] = _QEventLoop


def signal(*types: Any) -> Any:
    """Factory for Qt signals."""
    return _pyqtSignal(*types)  # type: ignore


def slot(*types: Any) -> Callable[..., Any]:
    """Decorator for synchronous Qt slots."""
    return _pyqtSlot(*types)  # type: ignore


def async_slot(*types: Any) -> Callable[[_T], _T]:
    """Decorator for asynchronous Qt slots."""
    return _asyncSlot(*types)  # type: ignore


def connect(signal: Callable[_P, Any], slot: Callable[_P, Any]) -> None:
    if not hasattr(signal, 'connect'):
        raise TypeError('signal must be a Qt signal')
    signal.connect(slot)  # type: ignore


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


class QAsync(QObject):
    """A helper object for maintaining the asyncio event loop in Qt.

    This class can be subclassed by other objects to wrap their async
    components in neat Qt-style classes.
    """

    def __init__(self, loop: asyncio.AbstractEventLoop,
                 parent: Optional[QWidget] = None) -> None:
        super().__init__(parent=parent)
        # Configure the event loop
        self.loop = loop or asyncio.get_event_loop()
        if not isinstance(self.loop, QEventLoop):
            raise RuntimeError(
                'Invalid asyncio loop type, QEventLoop required.')
        # Schedule setup hook
        # TODO: Figure out if there is a reason to do this through a slot
        # rather than a "raw" asyncio task.
        self.loop.create_task(self.async_setup())
        # Schedule cleanup hook
        connect(self.destroyed, self._slot_async_cleanup)

    @async_slot
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


_get_event_loop_old = pypresence_utils.get_event_loop  # type: ignore


def _get_event_loop(force_fresh: bool = False) -> asyncio.AbstractEventLoop:
    """Monkey-patched version of pypresence.utils.get_event_loop.

    pypresence insists that the win32 event loop must of a particular
    subclass, which is not compatible with the QEventLoop helper
    requried for Qt support.
    """
    if sys.platform == 'win32':
        loop = get_qasync_event_loop()
        if not loop.is_closed():
            return loop
    return _get_event_loop_old(force_fresh)


pypresence_utils.get_event_loop = _get_event_loop
