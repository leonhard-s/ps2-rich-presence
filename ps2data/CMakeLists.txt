cmake_minimum_required(VERSION 3.0)

project(Ps2GameData VERSION 0.1)

add_library(Ps2Data STATIC
  "class.hpp"
  "class.cpp"
  "faction.hpp"
  "faction.cpp"
  "id-types.hpp"
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
