cmake_minimum_required(VERSION 3.20)
project(ml64fx)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(initProject)
setup_os_vars()
include(importNode)

set(SRC "src/main.cpp")
if(WIN32)
    set(PLATFORM_SRC "app.manifest")
endif()

add_library(ml64fx SHARED ${SRC} ${PLATFORM_SRC})
configure_node_module(ml64fx)
