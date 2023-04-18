# <img src="https://raw.githubusercontent.com/leonhard-s/ps2-rich-presence/main/assets/icon.png" align="left" height="140"/>PS2 Rich Presence

An open source project providing [Rich Presence](https://discord.com/rich-presence) for [PlanetSide 2](https://www.planetside2.com/home).

<!-- TODO: Enable build info repository shield  -->
<!-- ![Build Workflow Status](https://img.shields.io/github/workflow/status/leonhard-s/ps2-rich-presence/build) -->

![License](https://img.shields.io/github/license/leonhard-s/ps2-rich-presence)
![GitHub Release Date](https://img.shields.io/github/release-date/leonhard-s/ps2-rich-presence?label=latest%20version)
<!-- ![Total Downloads](https://img.shields.io/github/downloads/leonhard-s/ps2-rich-presence/total) -->

[Overview](#overview) • [Features](#features) • [Limitations](#limitations) • [License](#license) • [Installation](#installation) • [Contributing](#contributing)

## Overview

The PlanetSide 2 Rich Presence Client is a project enabling you to display the current status of your PS2 character on [Discord](https://discord.com/).

![Rich Presence Comparison](https://user-images.githubusercontent.com/35902139/161334064-8f45dea9-60c4-489d-9b9f-c8963fea2b6c.png)

This application runs independently of the main PlanetSide 2 executable and does not affect your in-game experience. Instead, it listens to the game's event stream and infers the current status of your character based on kills and experience gain.

***Note:** This is a third-party tool and not affiliated with [Rogue Planet Games](https://www.rogueplanetgaming.com/home) or the PlanetSide 2 development team in any way.*

## Work In Progress

This utility is still in its early stages of development. Many features are missing (see below) and bugs are next to certain.

If you find some, please do not hesitate to [report them](https://github.com/leonhard-s/ps2-rich-presence/issues).

### Noteworthy omissions

These are planned features that are in development but not yet available in the current version:

- No start-on-login support
- No minimizing to tray
- No automatic tracking of characters on login or failover when they log off

## Features

- Display your character's status on Discord
- Detection of current continent, class, and vehicle
- Lightweight and low bandwidth usage
- ~~Automatic tracking upon login~~ (soon)

## Limitations

This application is not tied to the game executable or your player account. Its only way to detect your character's status is to listen to the game's event stream, and it is limited to the events and activities it can detect.

This means that there are things that it cannot detect, such as:

- Whether the player is currently in a squad or platoon (at least with any accuracy)
- Where on the map they are
- Any situation where you are not gaining any experience for an extended period of time

Additionally, we intentionally do not support display of character or outfit names for privacy reasons as there is no way to ensure that the tracked character belongs to the player who is running the application.

## License

The PS2 Rich Presence Client is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).

We do not recommend basing your own work on this project, but feel free to use it as a reference for your own projects or reach out to the developers if you have any questions.

### Game Artwork and Icons

Most artworks used in this application are based on assets available through the [Daybreak Game Company Census API](http://census.daybreakgames.com/), which were then modified (e.g., upscaled, traced, or cropped) for our requirements. See the [Assets README](https://github.com/leonhard-s/ps2-rich-presence/tree/main/assets) for details on where these assets came from and how exactly they were altered.

Use of our modifications is permitted without restriction. Note that your use still has to comply with the [Census API Policy](http://census.daybreakgames.com/#services-api-policy) and the [Daybreak Game Company Terms of Service](https://www.daybreakgames.com/terms-of-service#section13).

### Screenshots

Some assets, such as the continent artworks, are derived from in-game screenshots taken by project contributors. You can find the full-size screenshots [here](https://github.com/leonhard-s/ps2-rich-presence/tree/main/assets/screenshots).

You are free to use and modify these screenshots for your own projects given compliance with the PlanetSide 2 [Terms of Service](https://www.daybreakgames.com/terms-of-service#section13).

## Installation

You can find an installer package for the latest version in the project [Releases](https://github.com/leonhard-s/ps2-rich-presence/releases).

### Building from Source

The following is a non-exhaustive guide to building this project from source.

1. This project uses [CMake](https://cmake.org/download/) to generate its build system. At least version 3.25 is required.

2. Dependencies are provided by [vcpkg](https://vcpkg.io/). See [the vcpkg docs](https://vcpkg.io/en/getting-started.html) for downloading and bootstrapping an installation.

3. Clone this repository using Git:

    ```bash
    git clone https://github.com/leonhard-s/ps2-rich-presence
    ```

    Set your current working directory to the root of the cloned project:

    ```bash
    cd ps2-rich-presence
    ```

4. The CMake presets for this project expect the environment variable `VCPKG_INSTALLATION_ROOT` to be set to your vcpkg installation. Alternatively, you can create a custom preset inheriting from `vcpkg` and override the `CMAKE_TOOLCHAIN_FILE` cache variable to point to your installation's CMake toolchain script.

4. Run CMake to configure the project:

    ```ps1
    cmake -S . -B ./build/ --preset "vcpkg-x64-windows"
    ```

    > **Note:** During initial configuration, the CMake toolchain will fetch and install the dependencies. Note that this includes building Qt, which may take some time on first configuration.

5. Build the project:

    ```ps1
    cmake --build ./build/ --target install
    ```

    The `--target install` suffix will automatically install the application into the `dist/<preset>` subdirectory, including any required dependencies.

## Contributing

If you encounter any issues using the app or would like to suggest a new feature or change, feel free to get in touch via the repository [issues](https://github.com/leonhard-s/ps2-rich-presence/issues).

Please also consider lack of clarity or undocumented features when creating issues.  
Using this application should be straightforward for anyone familiar with the game, and any improvement to make the app clearer and more intuitive to use is worth discussing.
