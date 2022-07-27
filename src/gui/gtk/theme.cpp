#include "core/convert.hpp"
#include "gui/gtk/util.hpp"
#include "gui/theme.hpp"
#include "imgui/convert.hpp"

using namespace core;
using namespace imgui;

namespace gui::theme {

static GObjectAutoPtr(GSettings, settings, nullptr);

void initialize(Napi::Env env) {
    if (!settings) settings.reset(g_settings_new("org.gnome.desktop.interface"));
    if (!settings) throw Napi::Error::New(env, "failed to initialize g_settings with schema id `org.gnome.desktop.interface`");
}

Napi::Value getColorScheme(const Napi::CallbackInfo& info) {
    const auto colorScheme = stringFromGString(g_settings_get_string(settings.get(), "color-scheme"));
    return fromS32(info.Env(), static_cast<int>(colorScheme == "prefer-dark" ? ColorScheme::PreferDark : ColorScheme::PreferLight));
}

Napi::Value getAccentColor(const Napi::CallbackInfo& info) {
    const auto gtkTheme = stringFromGString(g_settings_get_string(settings.get(), "gtk-theme"));
    if (gtkTheme.find("bark") != std::string::npos) { return fromColor(info.Env(), { 120, 120, 89 }); }
    else if (gtkTheme.find("sage") != std::string::npos) { return fromColor(info.Env(), { 101, 123, 105 }); }
    else if (gtkTheme.find("olive") != std::string::npos) { return fromColor(info.Env(), { 75, 133, 1 }); }
    else if (gtkTheme.find("viridian") != std::string::npos) { return fromColor(info.Env(), { 3, 135, 91 }); }
    else if (gtkTheme.find("prussiangreen") != std::string::npos) { return fromColor(info.Env(), { 48, 130, 128 }); }
    else if (gtkTheme.find("purple") != std::string::npos) { return fromColor(info.Env(), { 119, 100, 216 }); }
    else if (gtkTheme.find("magenta") != std::string::npos) { return fromColor(info.Env(), { 179, 76, 179 }); }
    else if (gtkTheme.find("red") != std::string::npos) { return fromColor(info.Env(), { 218, 52, 80 }); }
    else if (gtkTheme.starts_with("Adwaita") || gtkTheme.find("blue") != std::string::npos) { return fromColor(info.Env(), { 0, 115, 229 }); }
    else if (gtkTheme.starts_with("Yaru") || gtkTheme.find("orange") != std::string::npos) { return fromColor(info.Env(), { 233, 84, 32 }); }
    else return fromColor(info.Env(), { 0, 115, 229 });
}

Napi::Value getGtkTheme(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), stringFromGString(g_settings_get_string(settings.get(), "gtk-theme")));
}

}
