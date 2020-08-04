cmake_minimum_required(VERSION 3.0)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/HunterGate.cmake)
HunterGate(
    URL "https://api.github.com/repos/You-i-Labs/Hunter/tarball/v0.23.190-youi8"
    HTTPHEADER "Authorization: token $ENV{GITHUB_ACCESS_TOKEN}"
    SHA1 "7e51c57baac5c95ac9788ebc0781f1619853cb07"
    FILEPATH "${CMAKE_CURRENT_LIST_DIR}/cmake/config.cmake"
)

project(easy_profiler CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif ()
else ()
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()

option(EASY_PROFILER_NO_GUI "Build easy_profiler without the GUI application (required Qt)" OFF)
option(EASY_PROFILER_NO_CONVERTER "Build easy_profiler without the converter" OFF)

set(EASY_PROGRAM_VERSION_MAJOR 2)
set(EASY_PROGRAM_VERSION_MINOR 1)
set(EASY_PROGRAM_VERSION_PATCH 0)
set(EASY_PRODUCT_VERSION_STRING "${EASY_PROGRAM_VERSION_MAJOR}.${EASY_PROGRAM_VERSION_MINOR}.${EASY_PROGRAM_VERSION_PATCH}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/sdk)

macro(easy_define_target_option TARGET SOURCE_OPTION TARGET_DEFINITION)
    if (${SOURCE_OPTION})
        set(_VALUE 1)
    else ()
        set(_VALUE 0)
    endif ()
    target_compile_options(${TARGET} PUBLIC -D${TARGET_DEFINITION}=${_VALUE})
endmacro()

SET(CMAKE_INSTALL_RPATH "$ORIGIN")

add_subdirectory(easy_profiler_core)
# If you want to build the GUI directly, this Release-prefix match fails and prevents the GUI from ever building on win32.
# If this is the case, use the commented out if() statement below.
#
# if (NOT EASY_PROFILER_NO_GUI)
if (NOT EASY_PROFILER_NO_GUI AND (NOT WIN64 OR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} MATCHES "Release-prefix"))
    # For WIN64, build only in release
    add_subdirectory(profiler_gui)
endif()
if (NOT EASY_PROFILER_NO_CONVERTER)
add_subdirectory(easy_profiler_converter)
endif()

if (NOT EASY_PROFILER_NO_SAMPLES)
    add_subdirectory(sample)
    add_subdirectory(reader)
endif ()
