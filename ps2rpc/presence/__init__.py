"""Classes and utilities specific to our presence format.

These components decide how a given game and/or user state will be
presented to the RPC. Note that this representation must match the
format expected by the Discord application.
"""

from ._factory import PresenceFactory
from ._payload import PresencePayload

__all__ = [
    'PresenceFactory',
    'PresencePayload',
]
