cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(Ps2GameData VERSION 0.2 LANGUAGES CXX)

add_library(Ps2Data STATIC
  "class.hpp"
  "class.cpp"
  "faction.hpp"
  "faction.cpp"
  "ps2.hpp"
  "server.hpp"
  "server.cpp"
  "vehicle.hpp"
  "vehicle.cpp"
  "zone.hpp"
  "zone.cpp"
)
target_include_directories(Ps2Data
  PUBLIC
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(Ps2Data
  PUBLIC
    Arx
)
set_target_properties(Ps2Data PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
