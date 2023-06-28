set (INCLUDE_DIRS ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include

    # Include BLAKE3 header files
    ${CMAKE_BINARY_DIR}/_deps/blake3-src/c/
)

# setup library source name
set(LIB_NAME "opcode_table")

# Add the source files
set (SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/opcode_table.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instruction_t.cpp
)

# set third party libraries
find_package(OpenSSL REQUIRED)
set (LIBS ${LIBS} OpenSSL::Crypto)

include_directories (${INCLUDE_DIRS})

add_library(${LIB_NAME} STATIC ${SRC_FILES})
target_link_libraries(${LIB_NAME} ${LIBS})
