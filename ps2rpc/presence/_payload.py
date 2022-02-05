"""Presence payload definition."""

import dataclasses
from typing import Any

__all__ = [
    'PresencePayload',
]


@dataclasses.dataclass(frozen=True)
class PresencePayload:
    """Payload sent to the Discord RPC API when updating the status.

    :param state: current playing status, e.g. "Looking to play"
    :param details: current game details, e.g. "Competitive"
    :param start: epoch seconds since start ("elapsed" counter)
    :param end: epoch seconds till end ("remaining" counter)
    :param large_image: key of the image asset to display
    :param large_text: text to display on the large image
    :param small_image: key of the image asset to display
    :param small_text: text to display on the small image
    """

    state: None | str
    details: None | str
    start: None | int
    end: None | int
    large_image: None | str
    large_text: None | str
    small_image: None | str
    small_text: None | str

    def update(self, **kwargs: Any) -> 'PresencePayload':
        """Create a new instance using the given values.

        The names supported for ``kwargs`` are the attribute names of
        the :class:`PresenceData` class.
        """
        return dataclasses.replace(self, **kwargs)

    def to_dict(self) -> dict[str, Any]:
        """Convert the data to a dict."""
        return dataclasses.asdict(self)
