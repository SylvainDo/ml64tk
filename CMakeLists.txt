cmake_minimum_required(VERSION 3.20)
project(ml64fx)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(initProject)
setup_os_vars()
include(importNode)

set(SRC
	"src/main.cpp"
	"src/core/type/convert.hpp"
	"src/core/type/instanceOf.hpp"
	)
if(WIN32)
    set(PLATFORM_SRC "app.manifest")
endif()

add_library(ml64fx SHARED ${SRC} ${PLATFORM_SRC})
target_include_directories(ml64fx PRIVATE "${CMAKE_SOURCE_DIR}/src")
configure_node_module(ml64fx)
