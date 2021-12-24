"""A utility module providing type hints for the RPC server.

Additionally, this also provides aliases for untyped functions to allow
using them in a typed context without requiring "#type: ignore"
comments everywhere.
"""

import asyncio
from typing import Any, Callable, TypeVar

from qasync import asyncSlot as _asyncSlot, QEventLoop as _QEventLoop
from PyQt6.QtWidgets import QApplication

__all__ = [
    'QEventLoop',
    'asyncSlot',
    'connect',
]

_T = TypeVar('_T')


class _EventLoopLike(asyncio.AbstractEventLoop):
    """A dummy class mimicing the QEventLoop interface.

    This class does nothing and is only as a type hint.
    """

    def __init__(self, *args: Any, **kwargs: Any) -> None:
        raise NotImplementedError()

    def __call__(self, app: QApplication) -> '_EventLoopLike':
        raise NotImplementedError()

    def __enter__(self) -> asyncio.AbstractEventLoop:
        raise NotImplementedError()

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> bool:
        raise NotImplementedError()


QEventLoop: _EventLoopLike = _QEventLoop


def asyncSlot(*args: Any) -> Callable[[_T], _T]:
    """Decorator for asynchronous Qt slots."""
    return _asyncSlot(*args)  # type: ignore


def connect(signal: Callable[..., Any], slot: Callable[..., Any]) -> None:
    if not hasattr(signal, 'connect'):
        raise TypeError('signal must be a Qt signal')
    signal.connect(slot)  # type: ignore
