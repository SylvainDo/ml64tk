#pragma once

#include <napi.h>

namespace gui::theme {

enum class ColorScheme {
    PreferLight,
    PreferDark
};

Napi::Value getColorScheme(const Napi::CallbackInfo& info);
Napi::Value getAccentColor(const Napi::CallbackInfo& info);

}
