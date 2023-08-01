add_subdirectory(crypto)

set (INCLUDE_DIRS ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CRYPTO_INCLUDE_DIR}
)
include_directories (${INCLUDE_DIRS})

# setup library source name
set(LIB_NAME "opcode_table")

# Add the source files
set (SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/opcode_table.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instruction_t.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/file_type_parser.cpp
)

# Add the library dependencies dir BLAKE3
set (LIBS ${LIBS}
    crypto
)

add_library(${LIB_NAME} STATIC ${SRC_FILES})
target_link_libraries(${LIB_NAME} ${LIBS})