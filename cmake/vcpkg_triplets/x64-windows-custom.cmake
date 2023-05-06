set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)

# NOTE: The Discord Game SDK is only available with dynamic linking
if(PORT STREQUAL "discord-game-sdk")
  set(VCPKG_LIBRARY_LINKAGE dynamic)
else()
  set(VCPKG_LIBRARY_LINKAGE static)
endif()
