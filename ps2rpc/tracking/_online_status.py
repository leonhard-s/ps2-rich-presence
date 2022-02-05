"""Utilities for tracking the online status of characters."""

import asyncio

import auraxium
from auraxium import event, ps2

__all__ = [
    'get_online_characters',
    'wait_for_login',
]


async def get_online_characters(character_ids: list[int],
                                client: auraxium.Client) -> list[int]:
    """Retrieve the online status for the given characters."""
    online = await ps2.Character.get_online(*character_ids, client=client)
    return [char.character_id for char in online]


async def wait_for_login(character_ids: list[int],
                         client: auraxium.EventClient) -> int:
    """Wait for any of the given characters to log into the game.

    This function returns as soon as the first character logs in.
    """
    flag = asyncio.Event()
    character_id: int = -1

    @client.trigger(
        event.PlayerLogin, characters=character_ids, single_shot=True)
    def on_login(event: event.PlayerLogin) -> None:
        nonlocal character_id
        character_id = event.character_id
        flag.set()

    _ = on_login
    # Wait for the first character to login
    await flag.wait()
    if character_id < 0:
        raise RuntimeError('An error occurred while waiting for a login')
    return character_id
