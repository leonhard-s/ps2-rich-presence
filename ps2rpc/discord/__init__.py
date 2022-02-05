"""This module handles data specific to the Discord application.

This includes the Discord application client ID, as well as a list of
known assets that can be used to validate the presence data prior to
sending it out.
"""

from ._appid import APP_ID
from ._assets import is_valid_asset_key

__all__ = [
    'APP_ID',
    'is_valid_asset_key',
]
