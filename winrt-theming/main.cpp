#include <winrt/Windows.UI.ViewManagement.h>

namespace wu = winrt::Windows::UI;
namespace wuvm = winrt::Windows::UI::ViewManagement;

inline bool isColorLight(const wu::Color& color) {
    return 5 * color.G + 2 * color.R + color.B > 8 * 128;
}

int getColorScheme() {
    wuvm::UISettings uiSettings;
    const auto color = uiSettings.GetColorValue(wuvm::UIColorType::Foreground);
    return isColorLight(color) ? 1 : 0;
}

void getAccentColor(float* v) {
    wuvm::UISettings uiSettings;
    const auto color = uiSettings.GetColorValue(wuvm::UIColorType::Accent);
    v[0] = color.R / 255.0f;
    v[1] = color.G / 255.0f;
    v[2] = color.B / 255.0f;
    v[3] = color.A / 255.0f;
}

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

void useImmersiveDarkMode(void* hWnd, bool enable) {
    BOOL value = enable;
    ::DwmSetWindowAttribute(static_cast<HWND>(hWnd), DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
}
