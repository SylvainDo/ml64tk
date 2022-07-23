#include "gui/dialogs.hpp"
#include "gui/gui.hpp"
#include "gui/theme.hpp"

namespace gui {

#ifdef _WIN32
namespace theme { Napi::Value useImmersiveDarkMode(const Napi::CallbackInfo& info); }
#else
namespace theme { Napi::Value getGtkTheme(const Napi::CallbackInfo& info); }
#endif

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

    return exports;
}

}
