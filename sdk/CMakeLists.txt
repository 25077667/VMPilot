# Disable Werror for retdec, only if not Windows (Windows doesn't support -Wno-error)
if (NOT WIN32)
    set (ORIG_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error")
endif()

# There are some `enum` value names "DEBUG" in retdec, which causes a conflict with our DEBUG macro
# We need to undefine it before including retdec headers
remove_definitions(-DDEBUG)

# The retdec has DCMAKE_INSTALL_PREFIX to /usr/local/include/capstone.
# We need to change it to our local directory
set (RETDEC_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/_deps/retdec-build)

# Apply the RETDEC_CMAKE_ARGS to the retdec package
CPMAddPackage(
    NAME retdec
    VERSION 5.1
    GITHUB_REPOSITORY "25077667/retdec"
    ${RETDEC_CMAKE_ARGS}
)
set (RETDEC_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/_deps/retdec-src/include)

# Recover the DEBUG macro
add_definitions(-DDEBUG=1)

# Recover original CXX flags
if (NOT WIN32)
    set (CMAKE_CXX_FLAGS "${ORIG_CMAKE_CXX_FLAGS}")
endif()

set (MAIN_FILE main.cpp)

set (TARGET_NAME VMPilot_SDK)

add_subdirectory(src)

add_executable(${TARGET_NAME} ${MAIN_FILE})
target_link_libraries(${TARGET_NAME} PRIVATE ${RETDEC_LIBRARIES} VMPilot_SDK_LIB)
