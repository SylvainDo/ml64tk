cmake_minimum_required(VERSION 3.20)
project(ml64fx)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(initProject)
setup_os_vars()
include(importGlad)
include(importImGui)
include(importNode)
if(UNIX)
	include(importGtk)
endif()

set(SRC
	"src/core/type/convert.hpp"
	"src/core/type/instanceOf.hpp"
	"src/core/byteswap.hpp"
	"src/core/largeInteger.hpp"
	"src/core/opaquePointer.cpp"
	"src/core/opaquePointer.hpp"
	"src/gui/dialogs.hpp"
	"src/gui/gui.cpp"
	"src/gui/gui.hpp"
	"src/gui/theme.hpp"
	"src/imgui/convert.hpp"
	"src/imgui/drawList.cpp"
	"src/imgui/drawList.hpp"
	"src/imgui/font.cpp"
	"src/imgui/font.hpp"
	"src/imgui/fontAtlas.cpp"
	"src/imgui/fontAtlas.hpp"
	"src/imgui/fontConfig.cpp"
	"src/imgui/fontConfig.hpp"
	"src/imgui/fontGlyphRangesBuilder.cpp"
	"src/imgui/fontGlyphRangesBuilder.hpp"
	"src/imgui/imgui.cpp"
	"src/imgui/imgui.hpp"
	"src/imgui/inputTextCallbackData.cpp"
	"src/imgui/inputTextCallbackData.hpp"
	"src/imgui/io.cpp"
	"src/imgui/io.hpp"
	"src/imgui/ioUserData.hpp"
	"src/imgui/keyData.cpp"
	"src/imgui/keyData.hpp"
	"src/imgui/payload.cpp"
	"src/imgui/payload.hpp"
	"src/imgui/sizeCallbackData.cpp"
	"src/imgui/sizeCallbackData.hpp"
	"src/imgui/style.cpp"
	"src/imgui/style.hpp"
	"src/imgui/tableColumnSortSpecs.cpp"
	"src/imgui/tableColumnSortSpecs.hpp"
	"src/imgui/tableSortSpecs.cpp"
	"src/imgui/tableSortSpecs.hpp"
	"src/imgui/viewport.cpp"
	"src/imgui/viewport.hpp"
	"src/imgui/windowClass.cpp"
	"src/imgui/windowClass.hpp"
	"src/appWindow.cpp"
	"src/appWindow.hpp"
	"src/main.cpp"
	)
if(WIN32)
    set(PLATFORM_SRC
		"src/gui/win32/dialogs.cpp"
		"app.manifest"
		)
elseif(UNIX)
	set(PLATFORM_SRC
		"src/gui/gtk/dialogs.cpp"
		"src/gui/gtk/theme.cpp"
		"src/gui/gtk/util.hpp"
		)
endif()

find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)

add_library(ml64fx SHARED ${SRC} ${PLATFORM_SRC})
target_include_directories(ml64fx PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(ml64fx PRIVATE fmt::fmt SDL2::SDL2 OpenGL::GL glad freetype imgui SDL2::SDL2_image SDL2::SDL2_ttf)
configure_node_module(ml64fx)

if(UNIX)
	add_subdirectory(napi-loader)
	target_link_libraries(ml64fx PRIVATE napi-loader gtk)
endif()
