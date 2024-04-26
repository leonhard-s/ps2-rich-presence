find_package(Microsoft.GSL CONFIG REQUIRED)

add_library(QtHelpers INTERFACE)
target_link_libraries(QtHelpers INTERFACE Microsoft.GSL::GSL)

# NOTE (leonhard-s): Using absolute path to prevent installation
target_include_directories(QtHelpers INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(QtHelpers INTERFACE
  include/adopt_pointer.h
  include/moc_macros.h
)
