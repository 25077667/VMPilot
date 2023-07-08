cmake_minimum_required(VERSION 3.20)

project(VMPilot)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
# Windows MSVC is a bit different, there might some error in the header files,
# so we disable the Wx flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Enable debug symbols, sanitizer, and disable optimization for debug build
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Disable sanitizer if compiler is ARM or target is ARM
        if (NOT (CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64"))
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer -O0")
        else ()
            message(WARNING "Sanitizer is not supported on ARM")
        endif ()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        # Enable sanitizer if MSVC version supports it
        if (CMAKE_CXX_FLAGS MATCHES ".*(/sdl|-sdl).*")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")
        else ()
            message(WARNING "Your MSVC version does not support sanitizer")
        endif ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /Od")
    endif ()
    # Define DEBUG macro
    add_compile_definitions(DEBUG)
endif ()


# If it is release build, we will enable optimization, also link time optimization if it is supported
if (CMAKE_BUILD_TYPE MATCHES "Release")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
        if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=auto")
        elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=thin")
        endif()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
        if (CMAKE_CXX_FLAGS MATCHES ".*(/GL|-GL).*") # If it is support link time optimization, we will enable it
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
        else ()
            message(WARNING "Your MSVC version does not support link time optimization")
        endif()
    endif()
    # Define NDEBUG macro
    add_compile_definitions(NDEBUG)
endif()

# Shrink symbol table if it is release build
if (CMAKE_BUILD_TYPE MATCHES "Release")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections -ffunction-sections")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gy")
    endif()
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
endif ()

# add dependencies
include(cmake/CPM.cmake)
CPMAddPackage("gh:nlohmann/json@3.11.2")
CPMAddPackage(
    NAME BLAKE3-cmake
    GITHUB_REPOSITORY 25077667/BLAKE3-cmake
    GIT_TAG 1.4.0
)
# Set BLAKE3 include directory variable, link BLAKE3 library
set(BLAKE3_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/blake3-src/c/)
set(BLAKE3_LIBRARY ${CMAKE_BINARY_DIR}/_deps/blake3-build/libblake3.a)

CPMAddPackage(
    NAME botan-cmake
    GIT_TAG 3.0.0-rc2
    GITHUB_REPOSITORY "25077667/botan-cmake"
)
# Set botan include directory variable, link botan library
set(BOTAN_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/botan-src/build/include/)
set(BOTAN_LIBRARY ${CMAKE_BINARY_DIR}/_deps/botan-src/libbotan-3.a)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(common)
add_subdirectory(sdk)
add_subdirectory(loader)
add_subdirectory(runtime)
