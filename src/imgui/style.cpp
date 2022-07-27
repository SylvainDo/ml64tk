#include "core/instanceOf.hpp"
#include "imgui/convert.hpp"
#include "imgui/style.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core;

namespace imgui {

Napi::FunctionReference Style::m_ctor;

Napi::Object Style::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "Style";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&Style::getTypeId>("typeId"),
        InstanceAccessor<&Style::toDebugString>("debugString"),
        InstanceMethod<&Style::ref>("ref"),
        InstanceMethod<&Style::unref>("unref"),

        InstanceMethod<&Style::equals>("equals"),
        InstanceAccessor<&Style::getAlpha, &Style::setAlpha>("alpha"),
        InstanceAccessor<&Style::getDisabledAlpha, &Style::setDisabledAlpha>("disabledAlpha"),
        InstanceAccessor<&Style::getWindowPadding, &Style::setWindowPadding>("windowPadding"),
        InstanceAccessor<&Style::getWindowRounding, &Style::setWindowRounding>("windowRounding"),
        InstanceAccessor<&Style::getWindowBorderSize, &Style::setWindowBorderSize>("windowBorderSize"),
        InstanceAccessor<&Style::getWindowMinSize, &Style::setWindowMinSize>("windowMinSize"),
        InstanceAccessor<&Style::getWindowTitleAlign, &Style::setWindowTitleAlign>("windowTitleAlign"),
        InstanceAccessor<&Style::getWindowMenuButtonPosition, &Style::setWindowMenuButtonPosition>("windowMenuButtonPosition"),
        InstanceAccessor<&Style::getChildRounding, &Style::setChildRounding>("childRounding"),
        InstanceAccessor<&Style::getChildBorderSize, &Style::setChildBorderSize>("childBorderSize"),
        InstanceAccessor<&Style::getPopupRounding, &Style::setPopupRounding>("popupRounding"),
        InstanceAccessor<&Style::getPopupBorderSize, &Style::setPopupBorderSize>("popupBorderSize"),
        InstanceAccessor<&Style::getFramePadding, &Style::setFramePadding>("framePadding"),
        InstanceAccessor<&Style::getFrameRounding, &Style::setFrameRounding>("frameRounding"),
        InstanceAccessor<&Style::getFrameBorderSize, &Style::setFrameBorderSize>("frameBorderSize"),
        InstanceAccessor<&Style::getItemSpacing, &Style::setItemSpacing>("itemSpacing"),
        InstanceAccessor<&Style::getItemInnerSpacing, &Style::setItemInnerSpacing>("itemInnerSpacing"),
        InstanceAccessor<&Style::getCellPadding, &Style::setCellPadding>("cellPadding"),
        InstanceAccessor<&Style::getTouchExtraPadding, &Style::setTouchExtraPadding>("touchExtraPadding"),
        InstanceAccessor<&Style::getIndentSpacing, &Style::setIndentSpacing>("indentSpacing"),
        InstanceAccessor<&Style::getColumnsMinSpacing, &Style::setColumnsMinSpacing>("columnsMinSpacing"),
        InstanceAccessor<&Style::getScrollbarSize, &Style::setScrollbarSize>("scrollbarSize"),
        InstanceAccessor<&Style::getScrollbarRounding, &Style::setScrollbarRounding>("scrollbarRounding"),
        InstanceAccessor<&Style::getGrabMinSize, &Style::setGrabMinSize>("grabMinSize"),
        InstanceAccessor<&Style::getGrabRounding, &Style::setGrabRounding>("grabRounding"),
        InstanceAccessor<&Style::getLogSliderDeadzone, &Style::setLogSliderDeadzone>("logSliderDeadzone"),
        InstanceAccessor<&Style::getTabRounding, &Style::setTabRounding>("tabRounding"),
        InstanceAccessor<&Style::getTabBorderSize, &Style::setTabBorderSize>("tabBorderSize"),
        InstanceAccessor<&Style::getTabMinWidthForCloseButton, &Style::setTabMinWidthForCloseButton>("tabMinWidthForCloseButton"),
        InstanceAccessor<&Style::getColorButtonPosition, &Style::setColorButtonPosition>("colorButtonPosition"),
        InstanceAccessor<&Style::getButtonTextAlign, &Style::setButtonTextAlign>("buttonTextAlign"),
        InstanceAccessor<&Style::getSelectableTextAlign, &Style::setSelectableTextAlign>("selectableTextAlign"),
        InstanceAccessor<&Style::getDisplayWindowPadding, &Style::setDisplayWindowPadding>("displayWindowPadding"),
        InstanceAccessor<&Style::getDisplaySafeAreaPadding, &Style::setDisplaySafeAreaPadding>("displaySafeAreaPadding"),
        InstanceAccessor<&Style::getMouseCursorScale, &Style::setMouseCursorScale>("mouseCursorScale"),
        InstanceAccessor<&Style::getAntiAliasedLines, &Style::setAntiAliasedLines>("antiAliasedLines"),
        InstanceAccessor<&Style::getAntiAliasedLinesUseTex, &Style::setAntiAliasedLinesUseTex>("antiAliasedLinesUseTex"),
        InstanceAccessor<&Style::getAntiAliasedFill, &Style::setAntiAliasedFill>("antiAliasedFill"),
        InstanceAccessor<&Style::getCurveTessellationTol, &Style::setCurveTessellationTol>("curveTessellationTol"),
        InstanceAccessor<&Style::getCircleTessellationMaxError, &Style::setCircleTessellationMaxError>("circleTessellationMaxError"),
        InstanceMethod<&Style::getColorAt>("getColorAt"),
        InstanceMethod<&Style::setColorAt>("setColorAt"),
        InstanceMethod<&Style::scaleAllSizes>("scaleAllSizes")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object Style::create(Napi::Env, ImGuiStyle* val) {
    const auto obj = m_ctor.New({ m_ctor.Value() });
    unwrap(obj)->m_val = val;
    return obj;
}

Style::Style(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Style>{ info } {
    if (info[0] != m_ctor.Value()) {
        m_val = new ImGuiStyle{};
        m_alloc = true;
    }
}

Style::~Style() {
    if (m_alloc) delete m_val;
}

Napi::Value Style::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<Style>(info.Env());
}

Napi::Value Style::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value Style::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value Style::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value Style::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == Style::get(info[0]));
}

Napi::Value Style::getAlpha(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->Alpha);
}

void Style::setAlpha(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->Alpha = asF32(val);
}

Napi::Value Style::getDisabledAlpha(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->DisabledAlpha);
}

void Style::setDisabledAlpha(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DisabledAlpha = asF32(val);
}

Napi::Value Style::getWindowPadding(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->WindowPadding);
}

void Style::setWindowPadding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WindowPadding = asVec2(val);
}

Napi::Value Style::getWindowRounding(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->WindowRounding);
}

void Style::setWindowRounding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WindowRounding = asF32(val);
}

Napi::Value Style::getWindowBorderSize(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->WindowBorderSize);
}

void Style::setWindowBorderSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WindowBorderSize = asF32(val);
}

Napi::Value Style::getWindowMinSize(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->WindowMinSize);
}

void Style::setWindowMinSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WindowMinSize = asVec2(val);
}

Napi::Value Style::getWindowTitleAlign(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->WindowTitleAlign);
}

void Style::setWindowTitleAlign(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WindowTitleAlign = asVec2(val);
}

Napi::Value Style::getWindowMenuButtonPosition(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->WindowMenuButtonPosition);
}

void Style::setWindowMenuButtonPosition(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WindowMenuButtonPosition = asS32(val);
}

Napi::Value Style::getChildRounding(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->ChildRounding);
}

void Style::setChildRounding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ChildRounding = asF32(val);
}

Napi::Value Style::getChildBorderSize(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->ChildBorderSize);
}

void Style::setChildBorderSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ChildBorderSize = asF32(val);
}

Napi::Value Style::getPopupRounding(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->PopupRounding);
}

void Style::setPopupRounding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->PopupRounding = asF32(val);
}

Napi::Value Style::getPopupBorderSize(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->PopupBorderSize);
}

void Style::setPopupBorderSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->PopupBorderSize = asF32(val);
}

Napi::Value Style::getFramePadding(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->FramePadding);
}

void Style::setFramePadding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->FramePadding = asVec2(val);
}

Napi::Value Style::getFrameRounding(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->FrameRounding);
}

void Style::setFrameRounding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->FrameRounding = asF32(val);
}

Napi::Value Style::getFrameBorderSize(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->FrameBorderSize);
}

void Style::setFrameBorderSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->FrameBorderSize = asF32(val);
}

Napi::Value Style::getItemSpacing(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->ItemSpacing);
}

void Style::setItemSpacing(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ItemSpacing = asVec2(val);
}

Napi::Value Style::getItemInnerSpacing(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->ItemInnerSpacing);
}

void Style::setItemInnerSpacing(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ItemInnerSpacing = asVec2(val);
}

Napi::Value Style::getCellPadding(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->CellPadding);
}

void Style::setCellPadding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->CellPadding = asVec2(val);
}

Napi::Value Style::getTouchExtraPadding(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->TouchExtraPadding);
}

void Style::setTouchExtraPadding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->TouchExtraPadding = asVec2(val);
}

Napi::Value Style::getIndentSpacing(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->IndentSpacing);
}

void Style::setIndentSpacing(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->IndentSpacing = asF32(val);
}

Napi::Value Style::getColumnsMinSpacing(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->ColumnsMinSpacing);
}

void Style::setColumnsMinSpacing(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ColumnsMinSpacing = asF32(val);
}

Napi::Value Style::getScrollbarSize(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->ScrollbarSize);
}

void Style::setScrollbarSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ScrollbarSize = asF32(val);
}

Napi::Value Style::getScrollbarRounding(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->ScrollbarRounding);
}

void Style::setScrollbarRounding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ScrollbarRounding = asF32(val);
}

Napi::Value Style::getGrabMinSize(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->GrabMinSize);
}

void Style::setGrabMinSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->GrabMinSize = asF32(val);
}

Napi::Value Style::getGrabRounding(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->GrabRounding);
}

void Style::setGrabRounding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->GrabRounding = asF32(val);
}

Napi::Value Style::getLogSliderDeadzone(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->LogSliderDeadzone);
}

void Style::setLogSliderDeadzone(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->LogSliderDeadzone = asF32(val);
}

Napi::Value Style::getTabRounding(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->TabRounding);
}

void Style::setTabRounding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->TabRounding = asF32(val);
}

Napi::Value Style::getTabBorderSize(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->TabBorderSize);
}

void Style::setTabBorderSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->TabBorderSize = asF32(val);
}

Napi::Value Style::getTabMinWidthForCloseButton(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->TabMinWidthForCloseButton);
}

void Style::setTabMinWidthForCloseButton(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->TabMinWidthForCloseButton = asF32(val);
}

Napi::Value Style::getColorButtonPosition(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->ColorButtonPosition);
}

void Style::setColorButtonPosition(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ColorButtonPosition = asS32(val);
}

Napi::Value Style::getButtonTextAlign(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->ButtonTextAlign);
}

void Style::setButtonTextAlign(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ButtonTextAlign = asVec2(val);
}

Napi::Value Style::getSelectableTextAlign(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->SelectableTextAlign);
}

void Style::setSelectableTextAlign(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->SelectableTextAlign = asVec2(val);
}

Napi::Value Style::getDisplayWindowPadding(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->DisplayWindowPadding);
}

void Style::setDisplayWindowPadding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DisplayWindowPadding = asVec2(val);
}

Napi::Value Style::getDisplaySafeAreaPadding(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->DisplaySafeAreaPadding);
}

void Style::setDisplaySafeAreaPadding(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DisplaySafeAreaPadding = asVec2(val);
}

Napi::Value Style::getMouseCursorScale(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->MouseCursorScale);
}

void Style::setMouseCursorScale(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MouseCursorScale = asF32(val);
}

Napi::Value Style::getAntiAliasedLines(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->AntiAliasedLines);
}

void Style::setAntiAliasedLines(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->AntiAliasedLines = asBool(val);
}

Napi::Value Style::getAntiAliasedLinesUseTex(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->AntiAliasedLinesUseTex);
}

void Style::setAntiAliasedLinesUseTex(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->AntiAliasedLinesUseTex = asBool(val);
}

Napi::Value Style::getAntiAliasedFill(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->AntiAliasedFill);
}

void Style::setAntiAliasedFill(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->AntiAliasedFill = asBool(val);
}

Napi::Value Style::getCurveTessellationTol(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->CurveTessellationTol);
}

void Style::setCurveTessellationTol(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->CurveTessellationTol = asF32(val);
}

Napi::Value Style::getCircleTessellationMaxError(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->CircleTessellationMaxError);
}

void Style::setCircleTessellationMaxError(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->CircleTessellationMaxError = asF32(val);
}

Napi::Value Style::getColorAt(const Napi::CallbackInfo& info) {
    return fromColor(info.Env(), m_val->Colors[asS32(info[0])]);
}

Napi::Value Style::setColorAt(const Napi::CallbackInfo& info) {
    m_val->Colors[asS32(info[0])] = asColor(info[1]);
    return info.Env().Undefined();
}

Napi::Value Style::scaleAllSizes(const Napi::CallbackInfo& info) {
    m_val->ScaleAllSizes(/* scale_factor */ asF32(info[0]));
    return info.Env().Undefined();
}

}
