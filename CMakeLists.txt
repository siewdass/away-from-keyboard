cmake_minimum_required( VERSION 3.9 )
project( afk )

#set( CMAKE_WARN_DEPRECATED OFF )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w" )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17" )
set(CMAKE_CXX_STANDARD 17)
set( SOURCES
     #${CMAKE_SOURCE_DIR}/utility.cpp
     ${CMAKE_SOURCE_DIR}/stats.cpp
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
     ${CMAKE_SOURCE_DIR}/global.cpp
     ${CMAKE_SOURCE_DIR}/location.cpp
     ${CMAKE_SOURCE_DIR}/main.cpp
)

#set( CMAKE_CXX_STANDARD_LIBRARIES "-static" )

#include_directories( ${CMAKE_SOURCE_DIR}/include )
#link_directories( ${CMAKE_SOURCE_DIR}/lib )

#set( OpenCV_DIR ${CMAKE_SOURCE_DIR}/cmake )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package( Threads REQUIRED )

find_package( PkgConfig REQUIRED )
pkg_check_modules( GTK3 REQUIRED gtk+-3.0 )
include_directories( ${GTK3_INCLUDE_DIRS} )

pkg_check_modules( APPINDICATOR appindicator3-0.1 )
include_directories( ${APPINDICATOR_INCLUDE_DIRS} )

pkg_check_modules( JSONCPP jsoncpp )
include_directories( ${Jsoncpp_INCLUDE_DIR} )

find_package( X11 REQUIRED )
include_directories( ${X11_INCLUDE_DIR} )

#message(STATUS "PRINT: ${APPINDICATOR_LIBRARIES}")

add_executable( afk ${SOURCES} )

target_link_libraries(
	afk
	${OpenCV_LIBS}
	xdo
	${GTK3_LIBRARIES}
	${APPINDICATOR_LIBRARIES}
	Threads::Threads
	${X11_LIBRARIES}
	${JSONCPP_LIBRARIES}
) 
