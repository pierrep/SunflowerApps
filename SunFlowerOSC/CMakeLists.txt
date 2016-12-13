cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

project (SunflowerDataService)

set (SunflowerDataService_VERSION_MAJOR 1)
set (SunflowerDataService_VERSION_MINOR 0)

find_package(CURL REQUIRED)
find_package(Boost REQUIRED)

include_directories (${CURL_INCLUDE_DIRS})
include_directories (${Boost_INCLUDE_DIRS})
include_directories (oscpack)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

SET (SOURCES
	"main.cpp"
	"Log.cpp"
	"Daemon.cpp"	
	"SerialPort.cpp"
	"PL40.cpp"
	"HTTPEmitter.cpp"
	"JSONFormatter.cpp"
        "OSCEmitter.cpp"
        "CSVReader.cpp"
)	

SET (HEADERS
	"Config.h"
	"Log.h"
	"Daemon.h"
	"SerialPort.h"
	"PL40.h"
	"IEmitter.h"	
	"JSONFormatter.h"
	"HTTPEmitter.h"
        "OSCEmitter.h"
        "CSVReader.h"
)	

file(GLOB OSCPACK
    "oscpack/osc/*.h"
    "oscpack/osc/*.cpp"
    "oscpack/ip/*.h"
    "oscpack/ip/*.cpp"
    "oscpack/ip/posix/*.h"
    "oscpack/ip/posix/*.cpp"
)


add_executable(SunflowerDataService ${SOURCES} ${HEADERS} ${OSCPACK})
target_link_libraries (SunflowerDataService ${CURL_LIBRARIES})
link_directories(${Boost_LIBRARY_DIRS})
