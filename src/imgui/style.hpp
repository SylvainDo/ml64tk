#pragma once

#include <napi.h>

struct ImGuiStyle;

namespace imgui {

class Style final : public Napi::ObjectWrap<Style> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env env, ImGuiStyle* val);

    static Style* unwrap(const Napi::Object& obj) {
        return Style::Unwrap(obj);
    }

    static Style* unwrap(const Napi::Value& val) {
        return Style::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiStyle* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiStyle* get() {
        return m_val;
    }

    Style(const Napi::CallbackInfo& info);
    virtual ~Style();

private:
    static Napi::FunctionReference m_ctor;
    ImGuiStyle* m_val;
    bool m_alloc{};

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getAlpha(const Napi::CallbackInfo& info);
    void setAlpha(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDisabledAlpha(const Napi::CallbackInfo& info);
    void setDisabledAlpha(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWindowPadding(const Napi::CallbackInfo& info);
    void setWindowPadding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWindowRounding(const Napi::CallbackInfo& info);
    void setWindowRounding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWindowBorderSize(const Napi::CallbackInfo& info);
    void setWindowBorderSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWindowMinSize(const Napi::CallbackInfo& info);
    void setWindowMinSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWindowTitleAlign(const Napi::CallbackInfo& info);
    void setWindowTitleAlign(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWindowMenuButtonPosition(const Napi::CallbackInfo& info);
    void setWindowMenuButtonPosition(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getChildRounding(const Napi::CallbackInfo& info);
    void setChildRounding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getChildBorderSize(const Napi::CallbackInfo& info);
    void setChildBorderSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getPopupRounding(const Napi::CallbackInfo& info);
    void setPopupRounding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getPopupBorderSize(const Napi::CallbackInfo& info);
    void setPopupBorderSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFramePadding(const Napi::CallbackInfo& info);
    void setFramePadding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFrameRounding(const Napi::CallbackInfo& info);
    void setFrameRounding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFrameBorderSize(const Napi::CallbackInfo& info);
    void setFrameBorderSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getItemSpacing(const Napi::CallbackInfo& info);
    void setItemSpacing(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getItemInnerSpacing(const Napi::CallbackInfo& info);
    void setItemInnerSpacing(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getCellPadding(const Napi::CallbackInfo& info);
    void setCellPadding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getTouchExtraPadding(const Napi::CallbackInfo& info);
    void setTouchExtraPadding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getIndentSpacing(const Napi::CallbackInfo& info);
    void setIndentSpacing(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getColumnsMinSpacing(const Napi::CallbackInfo& info);
    void setColumnsMinSpacing(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getScrollbarSize(const Napi::CallbackInfo& info);
    void setScrollbarSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getScrollbarRounding(const Napi::CallbackInfo& info);
    void setScrollbarRounding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getGrabMinSize(const Napi::CallbackInfo& info);
    void setGrabMinSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getGrabRounding(const Napi::CallbackInfo& info);
    void setGrabRounding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getLogSliderDeadzone(const Napi::CallbackInfo& info);
    void setLogSliderDeadzone(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getTabRounding(const Napi::CallbackInfo& info);
    void setTabRounding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getTabBorderSize(const Napi::CallbackInfo& info);
    void setTabBorderSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getTabMinWidthForCloseButton(const Napi::CallbackInfo& info);
    void setTabMinWidthForCloseButton(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getColorButtonPosition(const Napi::CallbackInfo& info);
    void setColorButtonPosition(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getButtonTextAlign(const Napi::CallbackInfo& info);
    void setButtonTextAlign(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getSelectableTextAlign(const Napi::CallbackInfo& info);
    void setSelectableTextAlign(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDisplayWindowPadding(const Napi::CallbackInfo& info);
    void setDisplayWindowPadding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDisplaySafeAreaPadding(const Napi::CallbackInfo& info);
    void setDisplaySafeAreaPadding(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMouseCursorScale(const Napi::CallbackInfo& info);
    void setMouseCursorScale(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getAntiAliasedLines(const Napi::CallbackInfo& info);
    void setAntiAliasedLines(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getAntiAliasedLinesUseTex(const Napi::CallbackInfo& info);
    void setAntiAliasedLinesUseTex(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getAntiAliasedFill(const Napi::CallbackInfo& info);
    void setAntiAliasedFill(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getCurveTessellationTol(const Napi::CallbackInfo& info);
    void setCurveTessellationTol(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getCircleTessellationMaxError(const Napi::CallbackInfo& info);
    void setCircleTessellationMaxError(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getColorAt(const Napi::CallbackInfo& info);
    Napi::Value setColorAt(const Napi::CallbackInfo& info);
    Napi::Value scaleAllSizes(const Napi::CallbackInfo& info);
};

}
