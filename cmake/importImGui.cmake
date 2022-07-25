set(DIR "${CMAKE_SOURCE_DIR}/vendor/imgui-docking")
set(CLUB_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui_club")

list(APPEND COMPILE_DEFS "IMGUI_DISABLE_OBSOLETE_FUNCTIONS")
list(APPEND COMPILE_DEFS "IMGUI_DISABLE_OBSOLETE_KEYIO")
list(APPEND COMPILE_DEFS "IMGUI_ENABLE_FREETYPE")

list(APPEND INC_DIRS "${DIR}")
list(APPEND INC_DIRS "${DIR}/backends")
list(APPEND INC_DIRS "${DIR}/misc/cpp")
list(APPEND INC_DIRS "${DIR}/misc/freetype")
list(APPEND INC_DIRS "${CLUB_DIR}/imgui_memory_editor")

set(SRC
	"${DIR}/backends/imgui_impl_opengl3_loader.h"
	"${DIR}/backends/imgui_impl_opengl3.cpp"
	"${DIR}/backends/imgui_impl_sdl.cpp"
	"${DIR}/backends/imgui_impl_sdl.h"
	"${DIR}/misc/cpp/imgui_stdlib.cpp"
	"${DIR}/misc/cpp/imgui_stdlib.h"
	"${DIR}/misc/freetype/imgui_freetype.cpp"
	"${DIR}/misc/freetype/imgui_freetype.h"
	"${DIR}/imconfig.h"
	"${DIR}/imgui_demo.cpp"
	"${DIR}/imgui_draw.cpp"
	"${DIR}/imgui_internal.h"
	"${DIR}/imgui_tables.cpp"
	"${DIR}/imgui_widgets.cpp"
	"${DIR}/imgui.cpp"
	"${DIR}/imgui.h"
	"${DIR}/imstb_rectpack.h"
	"${DIR}/imstb_textedit.h"
	"${DIR}/imstb_truetype.h"
	"${CLUB_DIR}/imgui_memory_editor/imgui_memory_editor.h")

add_library(imgui INTERFACE IMPORTED)
set_target_properties(imgui PROPERTIES
	INTERFACE_COMPILE_DEFINITIONS "${COMPILE_DEFS}"
	INTERFACE_INCLUDE_DIRECTORIES "${INC_DIRS}"
	INTERFACE_SOURCES "${SRC}")
set_source_files_properties(${SRC} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

find_package(freetype CONFIG REQUIRED)
target_link_libraries(imgui INTERFACE freetype)
