
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
project(                    Utilities               VERSION 1.0.0 LANGUAGES CXX)

add_compile_options(        $<$<CXX_COMPILER_ID:GNU>:-Wall>
                            $<$<CXX_COMPILER_ID:GNU>:-Wextra>
                            $<$<CXX_COMPILER_ID:GNU>:-Wpedantic>
                            $<$<CXX_COMPILER_ID:Clang>:-Wall>
                            $<$<CXX_COMPILER_ID:Clang>:-Wextra>
                            $<$<CXX_COMPILER_ID:Clang>:-Wpedantic>
                            $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_library(                Utilities INTERFACE)
target_compile_features(    Utilities INTERFACE     cxx_std_23)
target_include_directories( Utilities INTERFACE     .)
export(TARGETS              Utilities FILE          Utilities-config.cmake)

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
    enable_testing()

    add_subdirectory(tests)
endif()