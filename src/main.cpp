#include "core/largeInteger.hpp"
#include "core/opaquePointer.hpp"
#include "imgui/imgui.hpp"
#include "appWindow.hpp"

#include <fmt/format.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifdef _WIN32
#include <shellscalingapi.h>
#pragma comment(lib, "shcore.lib")

void setDpiAware() {
    if (SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE) != S_OK)
        SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
}

static struct AutoDpiAware {
    AutoDpiAware() {
        setDpiAware();
    }
} autoDpiAware;

extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif

void initialize(Napi::Env env) {
    SDL_SetMainReady();
    SDL_SetHint(SDL_HINT_APP_NAME, "ml64fx");
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw Napi::Error::New(env, fmt::format("failed to initialize sdl2: {}", SDL_GetError()));
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        throw Napi::Error::New(env, fmt::format("failed to initialize sdl2 image: {}", IMG_GetError()));
    if (TTF_Init() != 0)
        throw Napi::Error::New(env, fmt::format("failed to initialize sdl2 ttf: {}", TTF_GetError()));
    SDL_DisableScreenSaver();
}

void deinitialize() {
    SDL_EnableScreenSaver();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

Napi::Object initModule(Napi::Env env, Napi::Object exports) {
    initialize(env);
    std::atexit(deinitialize);

    core::SignedLargeInteger::initialize(env, exports);
    core::UnsignedLargeInteger::initialize(env, exports);
    core::OpaquePointer::initialize(env, exports);
    exports.Set("ImGui", imgui::initialize(env, Napi::Object::New(env)));
    AppWindow::initialize(env, exports);

    return exports;
}

NODE_API_MODULE(ml64fxModule, initModule)
