set(BUILD_TESTING OFF)
set(DbgCensus_INSTALL OFF)
set(BUILD_SHARED_LIBS OFF)

include(FetchContent)
FetchContent_Declare(DbgCensus SYSTEM
    GIT_REPOSITORY https://github.com/leonhard-s/dbg-census-cpp
    GIT_TAG "main"
)
FetchContent_MakeAvailable(DbgCensus)
