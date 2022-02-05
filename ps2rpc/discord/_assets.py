"""Lists all named assets available in the Discord application.

These keys are used to ensure we only specify known assets in the
presence data. Be sure to update this list when new assets are added to
the Discord application.
"""

__all__ = [
    'ASSET_KEYS',
    'is_valid_asset_key',
]

ASSET_KEYS = [
    # Infantry classes
    'infiltrator',
    'light_assault',
    'combat_medic',
    'engineer',
    'heavy_assult',
    'max',
    # Vehicles
    'ant',
    'bastion',
    'chimera',
    'colossus',
    'dervish',
    'engineer',
    'flash',
    'galaxy',
    'harasser',
    'javelin',
    'liberator',
    'lightning',
    'magrider',
    'mosquito',
    'prowler',
    'reaver',
    'scythe',
    'sunderer',
    'valkyrie',
    'vanguard',
    # Zones
    # TODO: Remove "zone_" prefix or add prefixes everywhere
    'zone_amerish',
    'zone_esamir',
    'zone_hossin',
    'zone_indar',
    'zone_oshur',
]


def is_valid_asset_key(key: str) -> bool:
    """Return whether the given key is a valid asset key."""
    return key in ASSET_KEYS
