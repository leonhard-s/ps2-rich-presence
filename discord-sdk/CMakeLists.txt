cmake_minimum_required(VERSION 3.0)

project(DiscordSDK VERSION 0.1)

add_library(Discord SHARED
  "include/discord.h"
  "include/types.h"
  "include/core.h"
  "include/application_manager.h"
  "include/user_manager.h"
  "include/image_manager.h"
  "include/activity_manager.h"
  "include/relationship_manager.h"
  "include/lobby_manager.h"
  "include/network_manager.h"
  "include/overlay_manager.h"
  "include/storage_manager.h"
  "include/store_manager.h"
  "include/voice_manager.h"
  "include/achievement_manager.h"
  "src/event.h"
  "src/ffi.h"
  "src/achievement_manager.cpp"
  "src/activity_manager.cpp"
  "src/application_manager.cpp"
  "src/core.cpp"
  "src/image_manager.cpp"
  "src/lobby_manager.cpp"
  "src/network_manager.cpp"
  "src/overlay_manager.cpp"
  "src/relationship_manager.cpp"
  "src/storage_manager.cpp"
  "src/store_manager.cpp"
  "src/types.cpp"
  "src/user_manager.cpp"
  "src/voice_manager.cpp"
)
target_include_directories(Discord
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)
