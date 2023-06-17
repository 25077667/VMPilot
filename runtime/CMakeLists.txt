SET (INCLUDE_DIRS ${INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add the source files
SET (SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/opcode_table.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instruction_t.cpp
)
SET (MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
SET (DUMP_OPTABLE ${CMAKE_CURRENT_SOURCE_DIR}/dump_optable.cpp)

# Set third party libraries
find_package(OpenSSL REQUIRED)
SET (LIBS ${LIBS} OpenSSL::Crypto nlohmann_json::nlohmann_json)

INCLUDE_DIRECTORIES (${INCLUDE_DIRS})
ADD_EXECUTABLE (runtime ${SRC_FILES} ${MAIN_FILE})
ADD_EXECUTABLE (dump_optable ${SRC_FILES} ${DUMP_OPTABLE})


# Link the executable to the library
TARGET_LINK_LIBRARIES (runtime ${LIBS})
TARGET_LINK_LIBRARIES (dump_optable ${LIBS})