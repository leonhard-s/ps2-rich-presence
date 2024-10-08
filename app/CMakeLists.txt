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
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF

  AUTOMOC ON
  OUTPUT_NAME "ps2-rich-presence"
)

install(TARGETS Ps2RichPresence
  RUNTIME DESTINATION "bin"
)

# Install
# -----------------------------------------------------------------------------

# Install Qt
qt6_generate_deploy_app_script(
  TARGET Ps2RichPresence
  FILENAME_VARIABLE QT_DEPLOY_SCRIPT
  NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${QT_DEPLOY_SCRIPT})
set(DEPENDENCIES_RELEASE
  libpng16.dll
  zlib1.dll
  freetype.dll
  double-conversion.dll
  pcre2-16.dll
  brotlidec.dll
  bz2.dll
  brotlicommon.dll
)
set(DEPENDENCIES_DEBUG
  libpng16d.dll
  zlibd1.dll
  freetyped.dll
  double-conversion.dll
  pcre2-16d.dll
  brotlidec.dll
  bz2d.dll
  brotlicommon.dll
)

foreach(DEP ${DEPENDENCIES_RELEASE})
  install(FILES "${Qt6_DIR}/../../bin/${DEP}" DESTINATION "bin"
    CONFIGURATIONS "Release" "RelWithDebInfo" "MinSizeRel"
  )
endforeach()

foreach(DEP ${DEPENDENCIES_DEBUG})
  install(FILES "${Qt6_DIR}/../../debug/bin/${DEP}" DESTINATION "bin"
    CONFIGURATIONS "Debug"
  )
endforeach()

# Discord SDK
install(FILES "${Discord_DLL_RELEASE}" DESTINATION "bin"
  CONFIGURATIONS "Release" "RelWithDebInfo" "MinSizeRel"
)
install(FILES "${Discord_DLL_DEBUG}" DESTINATION "bin"
  CONFIGURATIONS "Debug"
)
