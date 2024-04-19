find_package(Qt6 COMPONENTS Core REQUIRED)

add_library(PresenceLib)
target_link_libraries(PresenceLib PRIVATE
  Qt6::Core
  DbgCensus::REST
)
set_target_properties(PresenceLib PROPERTIES
  AUTOMOC ON
)
target_include_directories(PresenceLib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_sources(PresenceLib PRIVATE
  include/api/rest_client.h
  src/rest_client.cpp
)
