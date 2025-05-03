cmake_minimum_required( VERSION 3.9 )
project( afk )

set( CMAKE_CXX_STANDARD 17 )

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

set( SOURCE ${CMAKE_SOURCE_DIR}/main.cpp )

add_executable(afk ${SOURCE})

target_link_libraries(
     afk
     ${GTK3_LIBRARIES}
     ${OpenCV_LIBS}
     xdo
     Threads::Threads
     ${X11_LIBRARIES}
     ${JSONCPP_LIBRARIES}
     Xrandr
)
