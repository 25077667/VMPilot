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
    ${CMAKE_CURRENT_SOURCE_DIR}/ELFHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PEHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MachOHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Strategy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/X86Handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NativeFunctionBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NativeSymbolTable.cpp
)

set (LIBS ${LIBS}
    ${RETDEC_LIBRARIES}
)

include_directories(${INCLUDE_DIRS})

add_library(${TARGER_NAME} STATIC ${SRC_FILES})

target_link_libraries(${TARGER_NAME} ${LIBS})