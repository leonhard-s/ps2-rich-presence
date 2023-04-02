#[=======================================================================[.rst:
FindDiscord
-----------

Finds the Discord Game SDK.

Note that this module is designed for the folder structure provided by the
vcpkg version of the Discord Game SDK. If you have downloaded the SDK manually,
it will likely have trouble distinguishing between the debug and release
versions of the library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Discord::GameSDK``
  The main Discord Game SDK library.

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``Discord_FOUND``
  True if the system has the Discord Game SDK libraries.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Discord_INCLUDE_DIR``
  The directory containing ``discord-game-sdk/discord.h``.
``Discord_DLL``
  The path to the Discord Game SDK DLL.
``Discord_LIBRARY``
  The path to the Discord Game SDK import library for linking against the DLL.
``Discord_CPP_LIBRARY``
  The path to the locally-built CPP library.

#]=======================================================================]

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

# NOTE: The binaries for the Discord Game SDK have the exact same name for
# both debug and release builds. We therefore have to make some assumptions
# about the directory structure relative to the include directory to
# differentiate between the two.
# This is not a problem for vcpkg, but if you have downloaded the SDK manually
# you may need to set these variables manually or modify this module to suit
# your needs.
find_path(Discord_INCLUDE_DIR
  NAMES "discord-game-sdk/discord.h"
)

# NOTE: I was unable to find any version information easily accessible from the
# SDK headers, so it is hard-coded here until I can figure out a better way.
set(Discord_VERSION "2.5.6")

# The subsequent steps require this directory to exist, so if this fails we
# have nothing else to do.
if(NOT Discord_INCLUDE_DIR)
  return()
endif()

# Find the vcpkg installation directory for the current triplet.
set(_Discord_VCPKG "${Discord_INCLUDE_DIR}/..")
cmake_path(NATIVE_PATH _Discord_VCPKG NORMALIZE _Discord_VCPKG)
message(DEBUG "VCPKG triplet installation dir: ${_Discord_VCPKG}")

# Find the Discord Game SDK DLL.
find_file(Discord_DLL_RELEASE
  NAMES "discord_game_sdk.dll"
  PATHS "${_Discord_VCPKG}/bin"
  NO_DEFAULT_PATH
)
find_file(Discord_DLL_DEBUG
  NAMES "discord_game_sdk.dll"
  PATHS "${_Discord_VCPKG}/debug/bin"
  NO_DEFAULT_PATH
)

# Find the Discord Game SDK import library.
find_library(Discord_LIBRARY_RELEASE
  NAMES "discord_game_sdk"
  PATHS "${_Discord_VCPKG}/lib"
  NO_DEFAULT_PATH
)
find_library(Discord_LIBRARY_DEBUG
  NAMES "discord_game_sdk"
  PATHS "${_Discord_VCPKG}/debug/lib"
  NO_DEFAULT_PATH
)
select_library_configurations(Discord)

# Find the locally-built Discord Game SDK CPP library.
find_library(Discord_CPP_LIBRARY_RELEASE
  NAMES "discord_game_sdk_cpp"
  PATHS "${_Discord_VCPKG}/lib"
  NO_DEFAULT_PATH
)
find_library(Discord_CPP_LIBRARY_DEBUG
  NAMES "discord_game_sdk_cpp"
  PATHS "${_Discord_VCPKG}/debug/lib"
  NO_DEFAULT_PATH
)
select_library_configurations(Discord_CPP)

# Check that we found everything we need.
find_package_handle_standard_args(Discord
  REQUIRED_VARS
    Discord_DLL_RELEASE
    Discord_DLL_DEBUG
    Discord_LIBRARY
    Discord_CPP_LIBRARY
  VERSION_VAR Discord_VERSION
)

mark_as_advanced(
  Discord_INCLUDE_DIR
  Discord_DLL_RELEASE
  Discord_DLL_DEBUG
)

if(Discord_FOUND)
  add_library(Discord::_CPP STATIC IMPORTED)
  set_target_properties(Discord::_CPP PROPERTIES
    IMPORTED_LOCATION_RELEASE ${Discord_CPP_LIBRARY_RELEASE}
    IMPORTED_LOCATION_DEBUG ${Discord_CPP_LIBRARY_DEBUG}
    IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
  )

  add_library(Discord::_DLL SHARED IMPORTED)
  set_target_properties(Discord::_DLL PROPERTIES
    IMPORTED_LOCATION_RELEASE ${Discord_DLL_RELEASE}
    IMPORTED_LOCATION_DEBUG ${Discord_DLL_DEBUG}
    IMPORTED_IMPLIB_RELEASE ${Discord_LIBRARY_RELEASE}
    IMPORTED_IMPLIB_DEBUG ${Discord_LIBRARY_DEBUG}
    IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
  )

  add_library(Discord::GameSDK INTERFACE IMPORTED)
  target_include_directories(Discord::GameSDK
    INTERFACE ${Discord_INCLUDE_DIR}
  )
  target_link_libraries(Discord::GameSDK
    INTERFACE
      Discord::_CPP
      Discord::_DLL
  )
endif()
