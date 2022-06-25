cmake_minimum_required( VERSION 3.9 )
project( afk )

#set( CMAKE_WARN_DEPRECATED OFF )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lX11 -lxdo -ljsoncpp -lpthread" )

set( SOURCES
     ${CMAKE_SOURCE_DIR}/script.cpp
     ${CMAKE_SOURCE_DIR}/status.cpp
     ${CMAKE_SOURCE_DIR}/acknow.cpp
     ${CMAKE_SOURCE_DIR}/viewport.cpp
     ${CMAKE_SOURCE_DIR}/screenshot.cpp
     ${CMAKE_SOURCE_DIR}/inputs.cpp
     ${CMAKE_SOURCE_DIR}/utils.cpp
     ${CMAKE_SOURCE_DIR}/actions.cpp
     ${CMAKE_SOURCE_DIR}/character.cpp
     ${CMAKE_SOURCE_DIR}/astar.cpp
     ${CMAKE_SOURCE_DIR}/path.cpp
     ${CMAKE_SOURCE_DIR}/licence.cpp
     ${CMAKE_SOURCE_DIR}/converter.cpp
     ${CMAKE_SOURCE_DIR}/global.cpp
     ${CMAKE_SOURCE_DIR}/main.cpp
)

#set( CMAKE_CXX_STANDARD_LIBRARIES "-static" )

#include_directories( ${CMAKE_SOURCE_DIR}/include )
#link_directories( ${CMAKE_SOURCE_DIR}/lib )

#set( OpenCV_DIR ${CMAKE_SOURCE_DIR}/cmake )

find_package( OpenCV REQUIRED )

add_executable( afk ${SOURCES} )

target_link_libraries( afk ${OpenCV_LIBS} )
