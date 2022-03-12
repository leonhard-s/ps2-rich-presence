cmake_minimum_required(VERSION 3.16)

project(Ps2RichPresence VERSION 0.2.1)
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/leonhard-s/ps2-rich-presence")

set(CMAKE_AUTOMOC ON)

# Qt
find_package(Qt6 6.2.0 CONFIG REQUIRED
  COMPONENTS
    Core
    Gui
    Network
    Widgets
)
# Discord Game SDK
include("${PROJECT_SOURCE_DIR}/cmake/FetchDiscord.cmake")
# PlanetSide 2 API wrapper
add_subdirectory(arx)
# Game-specific data library
add_subdirectory(ps2data)

# Process config header; this lets us access project name and version strings
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ps2rpc/config.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/ps2rpc/config.hpp
  @ONLY
)
# Application icon
if(WIN32)
  set(APP_ICON_RESOURCE "${PROJECT_SOURCE_DIR}/windows/ps2rpc.rc")
endif()
# Main executable
add_executable(Ps2RichPresence WIN32
  "ps2rpc/appdata/appid.hpp"
  "ps2rpc/appdata/assets.hpp"
  "ps2rpc/appdata/assets.cpp"
  "ps2rpc/appdata/serviceid.hpp"
  "ps2rpc/gui/character-manager.hpp"
  "ps2rpc/gui/character-manager.cpp"
  "ps2rpc/gui/main-window.hpp"
  "ps2rpc/gui/main-window.cpp"
  "ps2rpc/gui/timeago.hpp"
  "ps2rpc/presence/factory.hpp"
  "ps2rpc/presence/factory.cpp"
  "ps2rpc/presence/handler.hpp"
  "ps2rpc/presence/handler.cpp"
  "ps2rpc/game/character-info.hpp"
  "ps2rpc/game/character-info.cpp"
  "ps2rpc/game/state.hpp"
  "ps2rpc/game/state.cpp"
  "ps2rpc/core.hpp"
  "ps2rpc/core.cpp"
  "ps2rpc/tracker.hpp"
  "ps2rpc/tracker.cpp"
  "ps2rpc/utils.hpp"
  "ps2rpc/utils.cpp"
  "ps2rpc/main.cpp"
  ${APP_ICON_RESOURCE}
)
target_include_directories(Ps2RichPresence
  PRIVATE
    ${PROJECT_SOURCE_DIR}/ps2rpc
    ${PROJECT_BINARY_DIR}/ps2rpc # Required for config.hpp.in
)
target_link_libraries(Ps2RichPresence
  PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Widgets
    Arx 
    discord_game_sdk
    Ps2Data
)
# Set executable name
set_target_properties(Ps2RichPresence
  PROPERTIES
    OUTPUT_NAME "PS2 Rich Presence"
)

# Default install to local build directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/local_install")

# Install application
install(TARGETS Ps2RichPresence
  RUNTIME DESTINATION "."
)
# Install Qt
get_target_property(ps2rpc_bin_name Ps2RichPresence OUTPUT_NAME)
install(CODE "execute_process(COMMAND 
    \"${windeployqt_exe}\" \"${CMAKE_INSTALL_PREFIX}/${ps2rpc_bin_name}.exe\"
  )"
)

# CPack Configuration

# Application name
set(CPACK_PACKAGE_NAME "PS2 Rich Presence")
# Vendor name
set(CPACK_PACKAGE_VENDOR "Leonhard S.")
# Application package description
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Rich Presence Support for PS2")
# Define installer filename
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME} v${CMAKE_PROJECT_VERSION}")
# Set default install folder name
set(CPACK_PACKAGE_INSTALL_DIRECTORY "PS2 Rich Presence")
# Package icon used during installation
set(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/windows/ps2rpc.ico")
# Specify license file (requires extension, hence a copy is made)
set(LICENSE_FILE "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.txt")
configure_file("${PROJECT_SOURCE_DIR}/LICENSE" "${LICENSE_FILE}" COPYONLY)
set(CPACK_RESOURCE_FILE_LICENSE "${LICENSE_FILE}")
# Disable component-based installation
set(CPACK_MONOLOTHIC_INSTALL TRUE)
# CPack generator to use
set(CPACK_GENERATOR "WIX")
# Specify the creation of start menu shortcuts
set(CPACK_PACKAGE_EXECUTABLES "PS2 Rich Presence" "PS2 Rich Presence")

# WIX Configuration

# Persistent GUID used to indentify past versions
set(CPACK_WIX_UPGRADE_GUID "2D6354D6-77D0-4502-9E37-0D00011BA511")
# Set product icon (used in installer and apps view)
set(CPACK_WIX_PRODUCT_ICON "${PROJECT_SOURCE_DIR}/windows/ps2rpc.ico")
# Disable start menu folder
set(CPACK_WIX_PROGRAM_MENU_FOLDER ".")
# Project info URL shown in the programs list
set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "${CMAKE_PROJECT_HOMEPAGE_URL}")

include(InstallRequiredSystemLibraries)
include(CPack)
