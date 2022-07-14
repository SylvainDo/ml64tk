set(SRC
	"main.cpp"
	"symbols.h")

add_library(napi-loader ${SRC})
target_link_libraries(napi-loader PRIVATE node-addon-api dl)
target_compile_options(napi-loader PRIVATE -fPIC)
