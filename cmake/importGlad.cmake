set(DIR "${CMAKE_SOURCE_DIR}/vendor/glad")
set(SRC "${DIR}/src/glad.c")

add_library(glad INTERFACE IMPORTED)
set_target_properties(glad PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${DIR}/include"
	INTERFACE_SOURCES "${SRC}")
set_source_files_properties(${SRC} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
