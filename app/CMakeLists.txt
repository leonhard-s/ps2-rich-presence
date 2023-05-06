cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(RichPresenceApp LANGUAGES CXX)

# Project setup
# -----------------------------------------------------------------------------

# Process config header; this lets us access project name and version strings
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.hpp"
  @ONLY
)

# Dependencies
# -----------------------------------------------------------------------------

# Discord Game SDK
find_package(Discord REQUIRED)

# Qt
find_package(Qt6 6.4 CONFIG REQUIRED
  COMPONENTS Core Gui Network Widgets WebSockets
)

# Targets
# -----------------------------------------------------------------------------
add_executable(Ps2RichPresence WIN32
  "appdata/appid.hpp"
  "appdata/assets.hpp"
  "appdata/assets.cpp"
  "appdata/service-id.hpp"
  "gui/character-manager.hpp"
  "gui/character-manager.cpp"
  "gui/main-window.hpp"
  "gui/main-window.cpp"
  "gui/timeago.hpp"
  "presence/factory.hpp"
  "presence/factory.cpp"
  "presence/handler.hpp"
  "presence/handler.cpp"
  "game/character-info.hpp"
  "game/character-info.cpp"
  "game/state.hpp"
  "game/state.cpp"
  "core.hpp"
  "core.cpp"
  "ess-client.hpp"
  "ess-client.cpp"
  "persistence.hpp"
  "persistence.cpp"
  "tracker.hpp"
  "tracker.cpp"
  "utils.hpp"
  "utils.cpp"
  "main.cpp"
  "$<$<PLATFORM_ID:Windows>:../windows/ps2rpc.rc>"
)
target_include_directories(Ps2RichPresence
  PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}"
)
target_link_libraries(Ps2RichPresence
  PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Widgets
    Qt::WebSockets
    Discord::GameSDK
    Ps2Data
    Arx
)
set_target_properties(Ps2RichPresence PROPERTIES
  AUTOMOC ON
  OUTPUT_NAME "ps2-rich-presence"
)

install(TARGETS Ps2RichPresence RUNTIME DESTINATION ".")

# Install
# -----------------------------------------------------------------------------

# Discord SDK
install(IMPORTED_RUNTIME_ARTIFACTS Discord::GameSDK DESTINATION ".")
