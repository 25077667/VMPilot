set (INCLUDE_DIRS 
    ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/segmentator/
    ${CMAKE_SOURCE_DIR}/common/include/
    ${RETDEC_INCLUDE_DIRS}
)

set (TARGER_NAME VMPilot_SDK_Segmentator)

set (SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/segmentator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pe_seg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/macho_seg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/elf_seg.cpp
)

set (LIBS ${LIBS}
    ${RETDEC_LIBRARIES}
)

include_directories(${INCLUDE_DIRS})

add_library(${TARGER_NAME} STATIC ${SRC_FILES})

target_link_libraries(${TARGER_NAME} ${LIBS})