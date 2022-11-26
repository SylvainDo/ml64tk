cmake_minimum_required(VERSION 3.20)
project(ml64tk)

option(NOGUI "Disable Gui module" OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(initProject)
setup_project_vars()
include(importGlad)
include(importImGui)
include(importNode)
if(NOT NOGUI)
    if(WIN32)
        include(importWinrtTheming)
    elseif(UNIX)
	    include(importGtk)
    endif()
endif()

set(SRC
	"src/audio/audio.cpp"
	"src/audio/audio.hpp"
	"src/audio/convert.hpp"
	"src/audio/listener.cpp"
	"src/audio/listener.hpp"
	"src/audio/music.cpp"
	"src/audio/music.hpp"
	"src/audio/sound.cpp"
	"src/audio/sound.hpp"
	"src/audio/soundBuffer.cpp"
	"src/audio/soundBuffer.hpp"
	"src/core/convert.hpp"
	"src/core/instanceOf.hpp"
	"src/core/largeInteger.hpp"
	"src/core/opaquePointer.cpp"
	"src/core/opaquePointer.hpp"
	"src/dollar/dollar.cpp"
	"src/dollar/dollar.hpp"
	"src/gfx/font.cpp"
	"src/gfx/font.hpp"
	"src/gfx/gfx.cpp"
	"src/gfx/gfx.hpp"
	"src/gfx/texture.cpp"
	"src/gfx/texture.hpp"
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
	"src/imgui/memoryEditor.cpp"
	"src/imgui/memoryEditor.hpp"
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
if(NOT NOGUI)
    set(GUI_SRC
        "src/gui/dialogs.hpp"
        "src/gui/gui.cpp"
        "src/gui/gui.hpp"
        "src/gui/theme.hpp"
        )

    if(WIN32)
        list(APPEND GUI_SRC
            "src/gui/win32/dialogs.cpp"
            "src/gui/win32/theme.cpp"
            "app.manifest"
            )
    elseif(UNIX)
        list(APPEND GUI_SRC
            "src/gui/gtk/dialogs.cpp"
            "src/gui/gtk/theme.cpp"
            "src/gui/gtk/util.hpp"
		    )
    endif()
endif()

find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SFML COMPONENTS audio CONFIG REQUIRED)

add_library(ml64tk SHARED ${SRC} ${GUI_SRC})
target_include_directories(ml64tk PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(ml64tk PRIVATE fmt::fmt SDL2::SDL2 OpenGL::GL glad imgui SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static sfml-audio)
configure_node_module(ml64tk)

if(WIN32 AND NOT NOGUI)
	add_dependencies(ml64tk winrt-theming)
	target_link_libraries(ml64tk PRIVATE winrt-theming::lib)
elseif(UNIX)
	add_subdirectory(napi-loader)
    target_link_libraries(ml64tk PRIVATE napi-loader)
    if(NOT NOGUI)
	    target_link_libraries(ml64tk PRIVATE gtk)
    endif()
endif()

if(NOGUI)
    target_compile_definitions(ml64tk PRIVATE NOGUI)
endif()

add_custom_command(TARGET ml64tk PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_SOURCE_DIR}/src/imconfig.h"
	"${CMAKE_SOURCE_DIR}/vendor/imgui-docking/imconfig.h")
