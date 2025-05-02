cmake_minimum_required( VERSION 3.9 )
project( afk )

set( CMAKE_CXX_STANDARD 17 )
set( SOURCES
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
    ${CMAKE_SOURCE_DIR}/global.cpp
    ${CMAKE_SOURCE_DIR}/location.cpp
    ${CMAKE_SOURCE_DIR}/main.cpp
)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package( Threads REQUIRED )

find_package( PkgConfig REQUIRED )
pkg_check_modules( GTK3 REQUIRED gtk+-3.0 )
include_directories( ${GTK3_INCLUDE_DIRS} )

pkg_check_modules( JSONCPP jsoncpp )
include_directories( ${Jsoncpp_INCLUDE_DIR} )

find_package( X11 REQUIRED )
include_directories( ${X11_INCLUDE_DIR} )

#add_executable( afk ${SOURCES} )

#target_link_libraries(
#	afk
#	${OpenCV_LIBS}
#	xdo
#	Threads::Threads
#	${X11_LIBRARIES}
#	${JSONCPP_LIBRARIES}
#) 

set( GUI_SOURCE
     ${CMAKE_SOURCE_DIR}/astar.cpp
     ${CMAKE_SOURCE_DIR}/path.cpp
     ${CMAKE_SOURCE_DIR}/actions.cpp
     ${CMAKE_SOURCE_DIR}/character.cpp
     ${CMAKE_SOURCE_DIR}/acknow.cpp
     ${CMAKE_SOURCE_DIR}/viewport.cpp
     ${CMAKE_SOURCE_DIR}/stats.cpp
     ${CMAKE_SOURCE_DIR}/location.cpp
     ${CMAKE_SOURCE_DIR}/new/inputs.cpp
     ${CMAKE_SOURCE_DIR}/new/screenshot.cpp
     ${CMAKE_SOURCE_DIR}/new/main.cpp
)

add_executable(gui ${GUI_SOURCE})

target_link_libraries(
     gui
     ${GTK3_LIBRARIES}
     ${OpenCV_LIBS}
     xdo
     Threads::Threads
     ${X11_LIBRARIES}
     ${JSONCPP_LIBRARIES}
     Xrandr
)
