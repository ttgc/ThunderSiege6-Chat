cmake_minimum_required(VERSION 3.12)

set(SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)

#Include the whole include directory for this directory
include_directories(${INCLUDE_FOLDER})

#Source files
set(SOURCES ${SOURCE_FOLDER}/thunderchatserver.cpp
			${SOURCE_FOLDER}/autonetworkselect.cpp
			${SOURCE_FOLDER}/clientdata.cpp)

#Headers files (for IntelliSense)
set(HEADERS ${INCLUDE_FOLDER}/thunderchatserver.hpp
			${INCLUDE_FOLDER}/autonetworkselect.hpp
			${INCLUDE_FOLDER}/clientdata.hpp)

#Main lib
add_library(Server ${SOURCES} ${HEADERS})
target_include_directories(Server SYSTEM INTERFACE ${INCLUDE_FOLDER})
target_link_libraries(Server PUBLIC Common)
