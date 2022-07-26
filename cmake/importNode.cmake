list(APPEND COMPILE_DEFINITIONS "NAPI_VERSION=8")
list(APPEND COMPILE_DEFINITIONS "NAPI_CPP_EXCEPTIONS")
if(WIN32)
    list(APPEND COMPILE_DEFINITIONS "HOST_BINARY=\"${NODE_HOST_BINARY}\"")
endif()

list(APPEND INCLUDE_DIRECTORIES "${NODE_DEV_DIR}/include")
list(APPEND INCLUDE_DIRECTORIES "${NODE_DEV_DIR}/include/node")
list(APPEND INCLUDE_DIRECTORIES "${NODE_ADDON_DEV_DIR}")

if(WIN32)
    list(APPEND LINK_LIBRARIES "${NODE_DEV_DIR}/lib/${TARGET_XARCH}/node.lib")
    list(APPEND LINK_LIBRARIES "delayimp.lib")
endif()

add_library(node-addon-api INTERFACE IMPORTED)
set_target_properties(node-addon-api PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS "${COMPILE_DEFINITIONS}"
    INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}")
if(WIN32)
    set_target_properties(node-addon-api PROPERTIES
        INTERFACE_LINK_LIBRARIES "${LINK_LIBRARIES}"
        INTERFACE_LINK_OPTIONS "/DELAYLOAD:${NODE_HOST_BINARY}"
        INTERFACE_SOURCES "${NODE_DEV_DIR}/src/win_delay_load_hook.cc")
endif()

function(configure_node_module target)
    target_link_libraries(${target} PRIVATE node-addon-api)
    set_target_properties(${target} PROPERTIES
        PREFIX ""
        SUFFIX "-${TARGET_PLATFORM}-${TARGET_ARCH}.node"
        PDB_NAME "${target}-${TARGET_PLATFORM}-${TARGET_ARCH}")
endfunction()
