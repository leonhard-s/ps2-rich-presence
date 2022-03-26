# This script is simply a stub to bypass the fact that CMake's
# "install(SCRIPT ...)" command does not provide an easy way of passing
# arguments or build configuration info to its script target.

set(project_root "${CMAKE_CURRENT_SOURCE_DIR}/../.")
set(target_exe_name "Ps2 Rich Presence")
set(windeployqt_temp "${project_root}/build/windeployqt_temp")

include("${project_root}/cmake/WindeployInstallHelper.cmake")

# Find windeployqt directory
file(READ "${project_root}/build/windeployqt_path.txt" windeployqt_path)
if (NOT EXISTS "${windeployqt_path}")
  message(FATAL_ERROR "windeployqt not found")
endif()

set(target_exe_path "${project_root}/build/Release/${target_exe_name}.exe")
if (NOT EXISTS "${target_exe_path}")
  message(WARNING "Could not locate built binary, windeployqt not run")
  return()
endif()

# Clear the temporary directory
if (EXISTS "${windeployqt_temp}")
  file(REMOVE "${windeployqt_temp}")
  file(MAKE_DIRECTORY "${windeployqt_temp}")
endif()

# Copy required Qt dependencies to temporary directory
windeployqt_install(
  "${windeployqt_path}"
  "${target_exe_path}"
  "${windeployqt_temp}"
)
