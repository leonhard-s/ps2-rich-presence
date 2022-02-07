# <img src="https://raw.githubusercontent.com/leonhard-s/ps2-rich-presence/main/assets/icon.png" align="left" height="140"/>PS2 Rich Presence

An open source utility providing [Discord Rich Presence](https://discord.com/rich-presence) for [PlanetSide 2](https://www.planetside2.com/home).

<!-- TODO: Add repository shields -->

<!-- TODO: Add example presence screenshots -->

[Overview](#overview) • [Features](#features) • [License / Disclaimer](#license) • [Installation](#installation) • [Contributing](#contributing)

## Rewrite Notice

This utility is mostly completed, but still in **proof-of-concept** stages.

More importantly, we want to focus on ease-of-use and portability, so we are currently **rewriting** it in C++.

If you want to play around with this version anyway, you can install and run it as follows:

1. Download and install [Python 3.10](https://www.python.org/downloads/) or higher.
2. Clone this repository using [Git](https://git-scm.com/).
3. Install the required dependencies using the `requirements.txt` file in the repository root:

    ```
    python -m pip install --user --upgrade -r ./path/to/requirements.txt
    ```
4. Run the app:

    ```
    python -m ps2rpc
    ```

## Overview

<!-- TODO: Add comparison of normal Discord game detection and Rich Presence -->

This application aims to provide a richer Discord experience for PlanetSide 2 players by integrating with Discord's Rich Presence API.

It does this by listening to the game's [Streaming API](https://census.daybreakgames.com/#what-is-websocket) to monitor your in-game activity, and then updating your Discord presence accordingly.

## Features

- Low-bandwidth requirements
- GUI interface for configuration
- Operates completely independenty from the PS2 installation\*
- Automatically enable Rich Presence when logging into the game
- Activity detection (class, vehicle, etc.)

*\*Since PS2RPC uses the API stream rather than the game's actual traffic, you do not even have to run this on the same device you're playing on. As long as there is a local Discord client and an internet connection, it will work.*

## License

### Source Code

This application uses [PyQt6](https://pypi.org/project/PyQt6/) and inherits its GPL-3.0 license. See [LICENSE](https://github.com/leonhard-s/ps2-rich-presence/blob/main/LICENSE) for details.

> **Note:**  
> For a more permissible license, considering basing your work the [Auraxium](https://github.com/leonhard-s/auraxium) Python package instead.
>
> Most of this project's complexity lies in getting the [asyncio](https://docs.python.org/3/library/asyncio.html)-based Auraxium client working in a [Qt](https://www.qt.io/product/qt6)-based application. Without GUI or Qt-related utilities, this entire application would only require a few dozen lines of code.

### Assets

Most artworks used in this application are based on assets available through the Daybreak Game Company [Census API](http://census.daybreakgames.com/), which were then modified (e.g. upscaled, traced, or cropped) for our requirements. See the [Assets README](https://github.com/leonhard-s/ps2-rich-presence/tree/main/assets) for details on what assets were used and how exactly they were altered.

Use of our modifications is permitted without restriction. Note that your use still has to comply with the [Census API Policy](http://census.daybreakgames.com/#services-api-policy) and the Daybreak Game Company [Terms of Service](https://www.daybreakgames.com/terms-of-service#section13).

## Installation

TBD

## Contributing

If you encounter any issues using PS2RPC or would like to suggest a new feature or change, feel free to get in touch via the repository [issues](https://github.com/leonhard-s/ps2-rich-presence/issues).

Please also consider lack of clarity or undocumented features when creating issues. Using this application should be as simple as launching the game itself, and any improvement to make the app clearer and more intuitive to use is worth discussing.
