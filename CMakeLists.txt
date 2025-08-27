
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")

    # Configure flags for mingw target
    if(WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target x86_64-w64-mingw32")
        set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -target x86_64-w64-mingw32")
    endif()
endif()

cmake_minimum_required(                             VERSION 3.21)
project(                    CxxUtils                VERSION 1.0.0 LANGUAGES CXX)

add_compile_options(        "$<$<CXX_COMPILER_ID:GNU>:-Wall;-Wextra;-Wpedantic>"
                            "$<$<CXX_COMPILER_ID:Clang>:-Wall;-Wextra;-Wpedantic>"
                            "$<$<CXX_COMPILER_ID:MSVC>:/W4>")

add_library(                CxxUtils  INTERFACE)
target_compile_features(    CxxUtils  INTERFACE     cxx_std_23)
target_include_directories( CxxUtils  INTERFACE     .)
export(TARGETS              CxxUtils  FILE          CxxUtils-config.cmake)

# Only compile the unit tests in the top-level project
if(PROJECT_IS_TOP_LEVEL)
    # Add GoogleTest using FetchContent
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    find_package(Threads REQUIRED)
    FetchContent_MakeAvailable(googletest)

    # Enable testing at project level
    include(CTest)
endif()

add_subdirectory(tests)
