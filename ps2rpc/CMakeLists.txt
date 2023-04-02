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
  "appdata/serviceid.hpp"
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
  "ess_client.hpp"
  "ess_client.cpp"
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
    "${PROJECT_BINARY_DIR}" # Required for config.hpp.in
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
set_target_properties(Ps2RichPresence
  PROPERTIES
    OUTPUT_NAME "ps2-rich-presence"
    AUTOMOC ON
)

install(TARGETS Ps2RichPresence
  RUNTIME DESTINATION "bin"
)

# Install
# -----------------------------------------------------------------------------

# TODO: Deployment for non-Windows platforms
if(WIN32)
  # Store location of windeployqt for use in dependent scripts
  set(windeployqt_path_file "${CMAKE_BINARY_DIR}/windeployqt_path.txt")
  get_target_property(qt_directory Qt6::Core IMPORTED_LOCATION)
  cmake_path(GET qt_directory PARENT_PATH windeployqt_dir)
  set(windeployqt_path "${windeployqt_dir}/windeployqt.exe")
  file(WRITE "${windeployqt_path_file}" "${windeployqt_path}")

  # Debug
  install(SCRIPT
    "${CMAKE_SOURCE_DIR}/cmake/QtInstallDebug.cmake"
    CONFIGURATIONS Debug
  )

  # Release
  install(SCRIPT
    "${CMAKE_SOURCE_DIR}/cmake/QtInstallRelease.cmake"
    CONFIGURATIONS Release
  )
  install(DIRECTORY
    "${CMAKE_BINARY_DIR}/windeployqt_temp/"
    DESTINATION "bin"
  )
else()
  message(STATUS "Qt installation is not yet supported on this platform")
endif()
