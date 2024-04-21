add_library(QtHelpers INTERFACE)

# NOTE (leonhard-s): Using absolute path to prevent installation
target_include_directories(QtHelpers INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(QtHelpers INTERFACE
  include/moc_macros.h
)
