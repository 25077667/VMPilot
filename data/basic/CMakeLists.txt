cmake_minimum_required(VERSION 3.20)

project(basic_binary)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

if("${CMAKE_CXX_FLAGS}" MATCHES "-m32")
    set(CMAKE_SYSTEM_PROCESSOR "x86") # Change CMAKE_SYSTEM_PROCESSOR to x86 if -m32 is set
endif()

add_executable(basic_binary main.cpp)
add_library(mock_VMPilot SHARED mock_VMPilot.cpp)

target_link_libraries(basic_binary mock_VMPilot)

# set output filename, mangle with platform and architecture
set(OUTPUT_SUBFIX ${CMAKE_SYSTEM_NAME}.${CMAKE_SYSTEM_PROCESSOR})
set_target_properties(basic_binary PROPERTIES OUTPUT_NAME basic_binary.${OUTPUT_SUBFIX})
set_target_properties(mock_VMPilot PROPERTIES OUTPUT_NAME mock_VMPilot.${OUTPUT_SUBFIX})

# copy output files to bin directory
set_target_properties(basic_binary PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(mock_VMPilot PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

