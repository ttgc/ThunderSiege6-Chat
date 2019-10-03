cmake_minimum_required(VERSION 3.12)

set(SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src)

#Include the whole include directory for this directory
include_directories(${INCLUDE_FOLDER})

#Source files
set(SOURCES ${SOURCE_FOLDER}/main.cpp)

#Main exec
add_executable(${PROJECT_NAME}-Client ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME}-Client PUBLIC Client)
