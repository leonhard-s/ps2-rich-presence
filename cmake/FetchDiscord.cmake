# This script will download the given version of the Discord Game SDK and
# create the imported target Discord::GameSDK from it.

# Script Configuration
set(DISCORD_SDK_URL "https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip")
set(DISCORD_SDK_DIR "${PROJECT_BINARY_DIR}/discord_game_sdk")
set(DISCORD_SDK_ARCHIVE "${PROJECT_BINARY_DIR}/discord_game_sdk.zip")

# Only download the SDK if it is missing
if (EXISTS "${DISCORD_SDK_DIR}" AND IS_DIRECTORY "${DISCORD_SDK_DIR}")
message(STATUS "Discord SDK found")
else()
# Download the SDK
message(STATUS "Discord SDK not found, downloading...")
file(DOWNLOAD "${DISCORD_SDK_URL}" "${DISCORD_SDK_ARCHIVE}" SHOW_PROGRESS)
# Extract the archive to known build dir
message(STATUS "Extracting SDK files")
file(ARCHIVE_EXTRACT
  INPUT "${DISCORD_SDK_ARCHIVE}"
  DESTINATION "${DISCORD_SDK_DIR}"
)
# Delete the ZIP archive
file(REMOVE "${DISCORD_SDK_ARCHIVE}")
endif()

# Locate appropariate libraries
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bit
    set(DISCORD_SDK_LIB_DIR "${DISCORD_SDK_DIR}/lib/x86_64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bit
    set(DISCORD_SDK_LIB_DIR "${DISCORD_SDK_DIR}/lib/x86")
else()
  message(FATAL_ERROR "Unsupported platform")
endif()

# Create library target
set(DISCORD_SDK_SRC "${DISCORD_SDK_DIR}/cpp")
add_library(discord_game_sdk STATIC
  "${DISCORD_SDK_SRC}/achievement_manager.cpp"
  "${DISCORD_SDK_SRC}/achievement_manager.h"
  "${DISCORD_SDK_SRC}/activity_manager.cpp"
  "${DISCORD_SDK_SRC}/activity_manager.h"
  "${DISCORD_SDK_SRC}/application_manager.cpp"
  "${DISCORD_SDK_SRC}/application_manager.h"
  "${DISCORD_SDK_SRC}/core.cpp"
  "${DISCORD_SDK_SRC}/core.h"
  "${DISCORD_SDK_SRC}/discord.h"
  "${DISCORD_SDK_SRC}/event.h"
  "${DISCORD_SDK_SRC}/ffi.h"
  "${DISCORD_SDK_SRC}/image_manager.cpp"
  "${DISCORD_SDK_SRC}/image_manager.h"
  "${DISCORD_SDK_SRC}/lobby_manager.cpp"
  "${DISCORD_SDK_SRC}/lobby_manager.h"
  "${DISCORD_SDK_SRC}/network_manager.cpp"
  "${DISCORD_SDK_SRC}/network_manager.h"
  "${DISCORD_SDK_SRC}/overlay_manager.cpp"
  "${DISCORD_SDK_SRC}/overlay_manager.h"
  "${DISCORD_SDK_SRC}/relationship_manager.cpp"
  "${DISCORD_SDK_SRC}/relationship_manager.h"
  "${DISCORD_SDK_SRC}/storage_manager.cpp"
  "${DISCORD_SDK_SRC}/storage_manager.h"
  "${DISCORD_SDK_SRC}/store_manager.cpp"
  "${DISCORD_SDK_SRC}/store_manager.h"
  "${DISCORD_SDK_SRC}/types.cpp"
  "${DISCORD_SDK_SRC}/types.h"
  "${DISCORD_SDK_SRC}/user_manager.cpp"
  "${DISCORD_SDK_SRC}/user_manager.h"
  "${DISCORD_SDK_SRC}/voice_manager.cpp"
  "${DISCORD_SDK_SRC}/voice_manager.h"
)
target_include_directories(discord_game_sdk
  PUBLIC
    "${DISCORD_SDK_SRC}"
)
target_link_libraries(discord_game_sdk
  PUBLIC
    "${DISCORD_SDK_LIB_DIR}/discord_game_sdk.dll.lib"
)
install(FILES "${DISCORD_SDK_LIB_DIR}/discord_game_sdk.dll"
  DESTINATION "."
)
