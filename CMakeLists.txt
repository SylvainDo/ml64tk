cmake_minimum_required(VERSION 3.20)
project(ml64fx)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(initProject)
setup_os_vars()
include(importGlad)
include(importImGui)
include(importNode)

set(SRC
	"src/main.cpp"
	"src/core/type/convert.hpp"
	"src/core/type/instanceOf.hpp"
	"src/core/byteswap.hpp"
	"src/core/largeInteger.hpp"
	)
if(WIN32)
    set(PLATFORM_SRC "app.manifest")
endif()

find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(freetype CONFIG REQUIRED)

add_library(ml64fx SHARED ${SRC} ${PLATFORM_SRC})
target_include_directories(ml64fx PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(ml64fx PRIVATE SDL2::SDL2 OpenGL::GL glad freetype imgui)
configure_node_module(ml64fx)
