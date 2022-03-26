# Defines the "windeployqt_install" function for use in other scripts.
# This locates the Qt installation, finds the windeployqt executable, and
# runs it on the provided "target_executable_path". The output is redirected to
# "windeployqt_temp_path" to be installed via a regular "install(FILES)" call.

function(windeployqt_install
    windeployqt_path
    target_executable_path
    windeployqt_temp_path
  )
  # Locate windeployqt
  if(NOT EXISTS "${windeployqt_path}")
    message(WARNING "windeployqt executable not found, unable to install qt")
    return()
  endif()
  message(STATUS "windeployqt found: ${windeployqt_path}")

  # Only proceed if the target executable exists
  if(NOT EXISTS "${target_executable_path}")
    message(WARNING "Target executable not found, unable to install qt")
    return()
  endif()
 
  # Create temporary directory
  if (NOT EXISTS "${windeployqt_temp_path}")
    message(STATUS "Creating temporary directory: ${windeployqt_temp_path}")
    file(MAKE_DIRECTORY "${windeployqt_temp_path}")
  endif()
  # Run windeployqt on the target executable
  message(STATUS "Running windeployqt on \"${target_executable_path}\"")
  execute_process(COMMAND ${windeployqt_path} --dir ${windeployqt_temp_path} ${target_executable_path}
    RESULT_VARIABLE windeployqt_result
  )
  if(NOT windeployqt_result EQUAL "0")
    message(WARNING "windeployqt exited with message: ${windeployqt_result}")
    return()
  endif()

endfunction()
