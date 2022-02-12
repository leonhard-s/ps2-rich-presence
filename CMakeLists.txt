cmake_minimum_required(VERSION 3.0)

project(Ps2RichPresence VERSION 0.2)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.2.3 REQUIRED
  COMPONENTS
    Core
    Gui
    Network
    Widgets
)

# First-party libraries
add_subdirectory(arx)
add_subdirectory(discord-sdk)
add_subdirectory(ps2data)

# Process config header; this lets us access project name and version strings
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ps2rpc/config.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/ps2rpc/config.hpp
  @ONLY
)
# Main executable
add_executable(Ps2RichPresence
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
  "ps2rpc/game/state.hpp"
  "ps2rpc/game/state.cpp"
  "ps2rpc/tracker.hpp"
  "ps2rpc/tracker.cpp"
  "ps2rpc/main.cpp"
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
    Discord
    Ps2Data
)
