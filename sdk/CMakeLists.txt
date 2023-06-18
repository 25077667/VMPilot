set (INCLUDE_DIRS 
    ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set (TARGET_NAME VMPilot_SDK)

# Add the source files
set (SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bytecode_compiler.cpp
)

set (LIBS ${LIBS} opcode_table)

include_directories(${INCLUDE_DIRS})
set (MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable (${TARGET_NAME} ${SRC_FILES} ${MAIN_FILE})

target_link_libraries(${TARGET_NAME} ${LIBS})