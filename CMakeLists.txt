
#set(ENV{PATH} "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64;$ENV{PATH}"


#set(CMAKE_CXX_COMPILER_TARGET   "x86_64-w64-mingw32")
#set(CMAKE_EXE_LINKER_FLAGS      "-fuse-ld=lld-link -Xlinker /LIBPATH:\"C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.41.34120/lib/x64\"")
#set(CMAKE_SHARED_LINKER_FLAGS   "-fuse-ld=lld-link -Xlinker /LIBPATH:\"C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.41.34120/lib/x64\"")
#set(CMAKE_CXX_FLAGS             "/vctoolsdir \"C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.41.34120\"")
#set(CMAKE_CXX_FLAGS             "--config ${CMAKE_CURRENT_SOURCE_DIR}/clang.cfg")

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Configure flags for mingw target
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target x86_64-w64-mingw32")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -target x86_64-w64-mingw32")
    
    # Don't link mingw32 to avoid main conflicts, let CMake handle the runtime
    # set(CMAKE_EXE_LINKER_FLAGS    "")
    # set(CMAKE_SHARED_LINKER_FLAGS "")
endif()

cmake_minimum_required(                             VERSION 3.20)
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
target_include_directories( Utilities INTERFACE     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                                    $<INSTALL_INTERFACE:include>
)
export(TARGETS              Utilities FILE          Utilities-config.cmake)


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
