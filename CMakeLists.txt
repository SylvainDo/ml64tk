cmake_minimum_required(VERSION 3.20)
project("NodeAddonTemplateCpp")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(initProject)
setup_os_vars()
include(importNode)

set(SRC "src/main.cpp")

add_library(addon SHARED ${SRC})
configure_node_module(addon)
