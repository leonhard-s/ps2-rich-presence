# This skript will download the given version of nlohmann's JSON library
# and create a CMake target for it.

# Script configuration
set(NLOHMANN_JSON_URL "https://github.com/nlohmann/json/releases/download/v3.10.5/json.hpp")
set(NLOHMANN_JSON_DIR "${PROJECT_BINARY_DIR}/nlohmann")
set(NLOHMANN_JSON_FILE "${NLOHMANN_JSON_DIR}/json.hpp")

# Only download the archive if it doesn't exist
if(EXISTS "${NLOHMANN_JSON_DIR}" AND IS_DIRECTORY "${NLOHMANN_JSON_DIR}")
message(STATUS "JSON found")
else()
# Download the header
message(STATUS "JSON not found, downloading...")
file(DOWNLOAD "${NLOHMANN_JSON_URL}" "${NLOHMANN_JSON_FILE}" SHOW_PROGRESS)
endif()

# Create library target
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json
  INTERFACE
    "${NLOHMANN_JSON_DIR}/.."
)
