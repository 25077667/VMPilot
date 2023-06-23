set (INCLUDE_DIRS 
    ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/bytecode_compiler/
    ${RETDEC_INCLUDE_DIRS}
)

set (TARGET_NAME VMPilot_SDK_Bytecode_Compiler)

# Add the source files
set (SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bytecode_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/x86_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/x86_64_compiler.cpp
)

set (LIBS ${LIBS} opcode_table nlohmann_json::nlohmann_json retdec::retdec)

include_directories(${INCLUDE_DIRS})

add_library(${TARGET_NAME} STATIC ${SRC_FILES})

target_link_libraries(${TARGET_NAME} ${LIBS})