#include "core/convert.hpp"
#include "gui/dialogs.hpp"
#include "gui/gui.hpp"
#include "gui/theme.hpp"

#include <fmt/format.h>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace core;

namespace gui {

#ifdef _WIN32
namespace theme { Napi::Value useImmersiveDarkMode(const Napi::CallbackInfo& info); }
#else
namespace theme { Napi::Value getGtkTheme(const Napi::CallbackInfo& info); }
#endif

Napi::Value openUrl(const Napi::CallbackInfo& info) {
    const auto url = asPath(info[0]);
#ifdef _WIN32
    if (ShellExecute(nullptr, nullptr, url.wstring().c_str(), nullptr, nullptr, SW_SHOW) <= (HINSTANCE)(INT_PTR)32)
#else
    if (std::system(fmt::format("xdg-open \"{}\"", url.string()).c_str()) != 0)
#endif
        throw Napi::Error::New(info.Env(), fmt::format("failed to open url `{}`", url.generic_string()));
    return info.Env().Undefined();
}

Napi::Object initialize(Napi::Env env, Napi::Object exports) {
    using namespace dialogs;
    exports.Set("getOpenFileName", Napi::Function::New(env, getOpenFileName));
    exports.Set("getOpenFileNames", Napi::Function::New(env, getOpenFileNames));
    exports.Set("getSaveFileName", Napi::Function::New(env, getSaveFileName));
    exports.Set("getExistingDirectory", Napi::Function::New(env, getExistingDirectory));
    exports.Set("getExistingDirectories", Napi::Function::New(env, getExistingDirectories));
    exports.Set("showMessageBox", Napi::Function::New(env, showMessageBox));

    using namespace theme;
    exports.Set("getColorScheme", Napi::Function::New(env, getColorScheme));
    exports.Set("getAccentColor", Napi::Function::New(env, getAccentColor));

#ifdef _WIN32
    exports.Set("useImmersiveDarkMode", Napi::Function::New(env, useImmersiveDarkMode));
#else
    exports.Set("getGtkTheme", Napi::Function::New(env, getGtkTheme));
#endif

    exports.Set("openUrl", Napi::Function::New(env, openUrl));

    return exports;
}

}
