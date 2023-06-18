set (INCLUDE_DIRS 
    ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common/include
)

# Add the source files
set (SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/decoder.cpp
)
set (MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
set (DUMP_OPTABLE ${CMAKE_CURRENT_SOURCE_DIR}/dump_optable.cpp)

# set third party libraries
set (LIBS ${LIBS} nlohmann_json::nlohmann_json opcode_table)

include_directories (${INCLUDE_DIRS})
add_executable (runtime ${SRC_FILES} ${MAIN_FILE})
add_executable (dump_optable ${SRC_FILES} ${DUMP_OPTABLE})

# Link the executable to the library
target_link_libraries (runtime ${LIBS})
target_link_libraries (dump_optable ${LIBS})