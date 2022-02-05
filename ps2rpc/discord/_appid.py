"""The Discord application ID targeted by the RPC.

This informs Discord what application we are using and which assets
are available.

For details, see <https://discordapp.com/developers/applications/>
"""

__all__ = [
    'APP_ID',
]

# Note that this is *not* sensitive data. It is used to identify the
# application and is not used for any other purpose; you can't use this
# to log into anything and it is owned to a specific Discord account.
APP_ID = 923122197793488997
