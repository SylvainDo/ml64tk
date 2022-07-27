#pragma once

#include <cstdio>
#include <cstdlib>

#define IM_ASSERT(_EXPR) \
do { \
    if (!(_EXPR)) { \
        fprintf(stderr, "ImGui assertion failed: %s: %d: %s: %s\n", __FILE__, __LINE__, __func__, #_EXPR); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#define IMGUI_DISABLE_OBSOLETE_KEYIO
#define IMGUI_ENABLE_FREETYPE
