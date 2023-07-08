set (INCLUDE_DIRS ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${BLAKE3_INCLUDE_DIR}
    ${BOTAN_INCLUDE_DIR}
)
include_directories (${INCLUDE_DIRS})

# setup library source name
set(LIB_NAME "opcode_table")

function(setup_crypto_library)
    # Set the crypto library
    set(CRYPTO_CRYPTO_CXX_FLAGS 
        -std=c++17
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/crypto.o
        COMMAND ${CMAKE_CXX_COMPILER} ${CRYPTO_CRYPTO_CXX_FLAGS} 
        -I ${CMAKE_CURRENT_SOURCE_DIR}/include
        -I ${BLAKE3_INCLUDE_DIR}
        -I ${BOTAN_INCLUDE_DIR}
        -c ${CMAKE_CURRENT_SOURCE_DIR}/src/crypto.cpp 
        -o ${CMAKE_CURRENT_BINARY_DIR}/crypto.o
    )
    # Invoke that command to build the library
    add_custom_target(crypto ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/crypto.o)
endfunction()

setup_crypto_library()

# Add the source files
set (SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/opcode_table.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instruction_t.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/file_type_parser.cpp
)

# Add the library dependencies dir BLAKE3
set (LIBS ${LIBS}
    ${BLAKE3_LIBRARY}
    ${BOTAN_LIBRARY}
)

add_library(${LIB_NAME} STATIC ${SRC_FILES})
target_link_libraries(${LIB_NAME} ${LIBS})
add_dependencies(${LIB_NAME} crypto)