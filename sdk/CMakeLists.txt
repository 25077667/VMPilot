set (MAIN_FILE main.cpp)

set (TARGET_NAME VMPilot_SDK)

add_subdirectory(src)

add_executable(${TARGET_NAME} ${MAIN_FILE})
target_link_libraries(${TARGET_NAME} PRIVATE ${RETDEC_LIBRARIES} VMPilot_SDK_LIB)
