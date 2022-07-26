include_external_msproject(winrt-theming "${CMAKE_SOURCE_DIR}/winrt-theming/winrt-theming.vcxproj")

add_library(winrt-theming::lib INTERFACE IMPORTED)
set_target_properties(winrt-theming::lib PROPERTIES
	INTERFACE_LINK_LIBRARIES "${CMAKE_SOURCE_DIR}/winrt-theming/${TARGET_XARCH}/${TARGET_BUILD_TYPE}/winrt-theming.lib")
