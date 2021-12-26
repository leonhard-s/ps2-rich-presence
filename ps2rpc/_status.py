from typing import Any, Dict, Optional
from ._presence import PresenceStatus
from ._ps2 import Ps2Class, Ps2Faction, Ps2Zone, Ps2Server


def _hide(key: str, kwargs: Dict[str, Any]) -> bool:
    return bool(kwargs.get(f'hide_{key}', False))


def generate_character_status(
        character: str,
        faction: Ps2Faction,
        outfit_tag: str,
        class_: Ps2Class,
        zone: Ps2Zone,
        server: Ps2Server,
        **kwargs: Any) -> PresenceStatus:
    """Generate a presence status for a character.

    Anonymous keyord arguments are used to control field visibility. By
    setting a `hide_<field>` key to `True`, the respective field will
    be hidden. For example, `hide_character` will hide the character
    name from the generated presence status.
    """
    # Large image
    large_image: Optional[str] = None
    large_text: Optional[str] = None
    if not _hide('zone', kwargs):
        large_image = f'zone_{zone.name.lower()}'
        large_text = zone.name.title()
    # Small image
    small_image: Optional[str] = None
    small_text: Optional[str] = None
    if not _hide('class', kwargs):
        small_image = f'class_{class_.name.lower()}'
        small_text = class_.name.title()
    # State
    state = 'Playing'
    if not _hide('faction', kwargs):
        state += f' {faction.name.upper()}'
    if not _hide('server', kwargs):
        state += f' on {server}'
    # Details
    details = ''
    if not _hide('character', kwargs):
        if not _hide('outfit', kwargs) and outfit_tag:
            details += f'[{outfit_tag}] '
        details += character
    # Generate payload
    return PresenceStatus(
        state=state,
        details=details,
        start=None,
        end=None,
        large_image=large_image,
        large_text=large_text,
        small_image=small_image,
        small_text=small_text)
