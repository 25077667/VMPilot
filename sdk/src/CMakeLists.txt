CPMAddPackage(
    NAME ELFIO
    GITHUB_REPOSITORY "serge1/ELFIO"
    GIT_TAG "Release_3.12"
)
CPMAddPackage("gh:gabime/spdlog@1.12.0")

set (INCLUDE_DIRS 
    ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/common/include
    ${CMAKE_BINARY_DIR}/_deps/elfio-src/
    # Add spdlog include
    ${spdlog_SOURCE_DIR}/include
)

set (TARGET_NAME VMPilot_SDK_LIB)
set (SRC_FILE ${SRC_FILE}
    ${CMAKE_CURRENT_SOURCE_DIR}/BytecodeCompileRecipe.cpp
)

include_directories(${INCLUDE_DIRS})
add_library(${TARGET_NAME} STATIC ${SRC_FILE})

add_subdirectory(bytecode_compiler)
add_subdirectory(segmentator)
set (LIBS ${LIBS}
    VMPilot_SDK_Bytecode_Compiler
)

target_link_libraries(${TARGET_NAME} PUBLIC ${LIBS})
