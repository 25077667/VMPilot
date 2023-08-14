# https://github.com/serge1/ELFIO/issues/124
# We would like to use the latest version of ELFIO from the author's repository
# We are waiting for the author to release a new version
# For now, we use the main_src tag for stability issues
CPMAddPackage(
    NAME ELFIO
    GITHUB_REPOSITORY "25077667/ELFIO"
    GIT_TAG "main_src"
)

set (INCLUDE_DIRS 
    ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/common/include
    ${CMAKE_BINARY_DIR}/_deps/elfio-src/
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
