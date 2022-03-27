cmake_minimum_required(VERSION 3.0)

project(Auraxium VERSION 0.1)

# JSON
include("${PROJECT_SOURCE_DIR}/../cmake/FetchJson.cmake")

add_library(Arx STATIC
  "include/arx/ps2_types.hpp"
  "include/arx/query.hpp"
  "include/arx/payload.hpp"
  "include/arx/support.hpp"
  "include/arx/types.hpp"
  "include/arx/urlgen.hpp"
  "include/arx.hpp"
  "include/arx/ess/endpoint.hpp"
  "include/arx/ess/events.hpp"
  "include/arx/ess/payload.hpp"
  "include/arx/ess/subscription.hpp"
  "include/arx/ess.hpp"
  "src/query.cpp"
  "src/payload.cpp"
  "src/support.cpp"
  "src/urlgen.cpp"
  "src/ess/endpoint.cpp"
  "src/ess/events.cpp"
  "src/ess/payload.cpp"
  "src/ess/subscription.cpp"
)
target_include_directories(Arx
  PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_BINARY_DIR}"
)
target_link_libraries(Arx
  PUBLIC
    nlohmann_json
)
