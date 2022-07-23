#include "core/type/convert.hpp"
#include "gui/theme.hpp"
#include "imgui/convert.hpp"
#include "imgui/viewport.hpp"

using namespace core::type::convert;
using namespace imgui;

int getColorScheme();
void getAccentColor(float* v);
void useImmersiveDarkMode(void* hWnd, bool enable);

namespace gui::theme {

Napi::Value getColorScheme(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ::getColorScheme());
}

Napi::Value getAccentColor(const Napi::CallbackInfo& info) {
    ImColor color;
    ::getAccentColor(&color.Value.x);
    return fromColor(info.Env(), color);
}

Napi::Value useImmersiveDarkMode(const Napi::CallbackInfo& info) {
    ::useImmersiveDarkMode(
        /* hWnd */ Viewport::get(info[0])->PlatformHandleRaw,
        /* enable */ asBool(info[1]));
    return info.Env().Undefined();
}

}
