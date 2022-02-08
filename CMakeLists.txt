cmake_minimum_required(VERSION 3.0)

project(Ps2RichPresence VERSION 0.1)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.2.3 REQUIRED
  COMPONENTS
    Core
)

# First-party libraries
add_subdirectory(arx)
add_subdirectory(ps2data)

# Main executable
add_executable(Ps2RichPresence
    ps2rpc/game/state.hpp
    ps2rpc/game/state.cpp
)
target_include_directories(Ps2RichPresence
  PRIVATE
    ${PROJECT_SOURCE_DIR}/ps2rpc
)
target_link_libraries(Ps2RichPresence
  PRIVATE
    Qt::Core
    Arx 
    Ps2Data
)
