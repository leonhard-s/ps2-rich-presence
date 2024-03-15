cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(Ps2RichPresence VERSION 0.4.0 LANGUAGES CXX)
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/leonhard-s/ps2-rich-presence")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Global setup
# -----------------------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") # MSVC compiler
  add_compile_options(
    /MP # Multi-processor compilation
    /Wall # All warnings

    # Disable warnings
    /wd4820 # Padding bytes added
    /wd5045 # Spectre mitigation
    /wd4866 # Evaluation order nonconformance in operator<< (compiler issue)

    /wd4710 # Function not inlined
    /wd4711 # Function selected for inline expansion
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND MSVC) # Clang compiler with MSVC frontend
  add_compile_options(
    /Wall # All warnings

    # Disable warnings
    -Wno-c++98-compat -Wno-c++98-compat-pedantic # C++98 compatibility
    -Wno-c++20-compat # pre-C++20 compatibility
    -Wno-covered-switch-default # Switch statement with unreachable default case
  )
else()
  add_compile_options(
    -Wall # All warnings
    -Wextra # Extra warnings
    -pedantic # Pedantic warnings
  )
endif()

# Dependencies
# -----------------------------------------------------------------------------
add_subdirectory(3rdparty)

# Targets
# -----------------------------------------------------------------------------

# Arx library, used for talking to the PlanetSide 2 API
add_subdirectory(arx)

# PS2Data library, handles custom static PlanetSide 2 Data

# TODO: Replace this with a TLRU caching layer that queries the PS2 REST API
# instead; the API has been updated with the missing data this library was
# originally created for.
add_subdirectory(ps2data)

# Main executable
add_subdirectory(app)

# Install
# -----------------------------------------------------------------------------

# License
install(FILES "${PROJECT_BINARY_DIR}/LICENSE.txt" DESTINATION ".")

# Packaging
# -----------------------------------------------------------------------------

# CPack configuration
set(CPACK_PACKAGE_NAME "PS2 Rich Presence")
set(CPACK_PACKAGE_VENDOR "Leonhard S.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Rich Presence Support for PS2")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME} v${CMAKE_PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "PS2 Rich Presence")
set(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/windows/ps2rpc.ico")

# Specify license file (requires extension, hence a copy is made)
set(LICENSE_FILE "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.txt")
configure_file("${PROJECT_SOURCE_DIR}/LICENSE" "${LICENSE_FILE}" COPYONLY)
set(CPACK_RESOURCE_FILE_LICENSE "${LICENSE_FILE}")

set(CPACK_MONOLOTHIC_INSTALL TRUE)
set(CPACK_GENERATOR "WIX")
set(CPACK_PACKAGE_EXECUTABLES "PS2 Rich Presence")

# WIX configuration
set(CPACK_WIX_UPGRADE_GUID "2D6354D6-77D0-4502-9E37-0D00011BA511")
set(CPACK_WIX_PRODUCT_ICON "${PROJECT_SOURCE_DIR}/windows/ps2rpc.ico")
set(CPACK_WIX_PROGRAM_MENU_FOLDER ".")
set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "${CMAKE_PROJECT_HOMEPAGE_URL}")

include(CPack)
