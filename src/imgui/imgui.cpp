#include "core/type/instanceOf.hpp"
#include "core/opaquePointer.hpp"
#include "imgui/convert.hpp"
#include "imgui/drawList.hpp"
#include "imgui/font.hpp"
#include "imgui/fontAtlas.hpp"
#include "imgui/fontConfig.hpp"
#include "imgui/fontGlyphRangesBuilder.hpp"
#include "imgui/imgui.hpp"
#include "imgui/inputTextCallbackData.hpp"
#include "imgui/io.hpp"
#include "imgui/ioUserData.hpp"
#include "imgui/keyData.hpp"
#include "imgui/memoryEditor.hpp"
#include "imgui/payload.hpp"
#include "imgui/sizeCallbackData.hpp"
#include "imgui/style.hpp"
#include "imgui/tableColumnSortSpecs.hpp"
#include "imgui/tableSortSpecs.hpp"
#include "imgui/viewport.hpp"
#include "imgui/windowClass.hpp"

#include <optional>

#include <imgui_internal.h>
#include <imgui_stdlib.h>

using namespace core;
using namespace core::type::convert;

namespace imgui {

template <class T, auto As, auto From>
class ValueRef {
public:
    ValueRef(const Napi::Value& val) {
        if (!val.IsUndefined()) {
            m_array = valueAsArray(val);
            m_value = As(m_array.Get(0u));
        }
    }

    ~ValueRef() {
        if (!m_array.IsUndefined()) {
            m_array.Set(0u, From(m_array.Env(), m_value));
        }
    }

    T* get() {
        return !m_array.IsUndefined() ? &m_value : nullptr;
    }

private:
    Napi::Array m_array;
    T m_value;
};

class ColorRef {
public:
    ColorRef(const Napi::Value& val) {
        if (!val.IsUndefined()) {
            m_object = valueAsObject(val);
            m_value = asColor(m_object);
        }
    }

    ~ColorRef() {
        if (!m_object.IsUndefined()) {
            m_object.Set("x", fromF32(m_object.Env(), m_value.Value.x));
            m_object.Set("y", fromF32(m_object.Env(), m_value.Value.y));
            m_object.Set("z", fromF32(m_object.Env(), m_value.Value.z));
            m_object.Set("w", fromF32(m_object.Env(), m_value.Value.w));
        }
    }

    float* get() {
        return !m_object.IsUndefined() ? &m_value.Value.x : nullptr;
    }

private:
    Napi::Object m_object;
    ImColor m_value;
};

using BoolRef = ValueRef<bool, asBool, fromBool>;
using IntRef = ValueRef<std::int32_t, asS32, fromS32>;
using UintRef = ValueRef<std::uint32_t, asU32, fromU32>;
using FloatRef = ValueRef<float, asF32, fromF32>;
using DoubleRef = ValueRef<double, asF64, fromF64>;
inline Napi::Value fromStrUtf8_(Napi::Env env, const std::string& value) { return fromStrUtf8(env, value); }
using StringRef = ValueRef<std::string, asStrUtf8, fromStrUtf8_>;

Napi::Value getIO(const Napi::CallbackInfo& info) {
    return IO::create(info.Env(), &ImGui::GetIO());
}

Napi::Value getStyle(const Napi::CallbackInfo& info) {
    return Style::create(info.Env(), &ImGui::GetStyle());
}

Napi::Value showDemoWindow(const Napi::CallbackInfo& info) {
    ImGui::ShowDemoWindow(/* p_open */ BoolRef{ info[0] }.get());
    return info.Env().Undefined();
}

Napi::Value showMetricsWindow(const Napi::CallbackInfo& info) {
    ImGui::ShowMetricsWindow(/* p_open */ BoolRef{ info[0] }.get());
    return info.Env().Undefined();
}

Napi::Value showDebugLogWindow(const Napi::CallbackInfo& info) {
    ImGui::ShowDebugLogWindow(/* p_open */ BoolRef{ info[0] }.get());
    return info.Env().Undefined();
}

Napi::Value showStackToolWindow(const Napi::CallbackInfo& info) {
    ImGui::ShowStackToolWindow(/* p_open */ BoolRef{ info[0] }.get());
    return info.Env().Undefined();
}

Napi::Value showAboutWindow(const Napi::CallbackInfo& info) {
    ImGui::ShowAboutWindow(/* p_open */ BoolRef{ info[0] }.get());
    return info.Env().Undefined();
}

Napi::Value showStyleEditor(const Napi::CallbackInfo& info) {
    ImGui::ShowStyleEditor(/* ref */ info[0].IsUndefined() ? nullptr : Style::get(info[0]));
    return info.Env().Undefined();
}

Napi::Value showStyleSelector(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::ShowStyleSelector(/* label */ asStrUtf8(info[0]).c_str()));
}

Napi::Value showFontSelector(const Napi::CallbackInfo& info) {
    ImGui::ShowFontSelector(/* label */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value showUserGuide(const Napi::CallbackInfo& info) {
    ImGui::ShowUserGuide();
    return info.Env().Undefined();
}

Napi::Value getVersion(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), ImGui::GetVersion());
}

Napi::Value styleColorsDark(const Napi::CallbackInfo& info) {
    ImGui::StyleColorsDark(/* dst */ info[0].IsUndefined() ? nullptr : Style::get(info[0]));
    return info.Env().Undefined();
}

Napi::Value styleColorsLight(const Napi::CallbackInfo& info) {
    ImGui::StyleColorsLight(/* dst */ info[0].IsUndefined() ? nullptr : Style::get(info[0]));
    return info.Env().Undefined();
}

Napi::Value styleColorsClassic(const Napi::CallbackInfo& info) {
    ImGui::StyleColorsClassic(/* dst */ info[0].IsUndefined() ? nullptr : Style::get(info[0]));
    return info.Env().Undefined();
}

Napi::Value begin(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::Begin(
        /* name */ asStrUtf8(info[0]).c_str(),
        /* p_open */ BoolRef{ info[1] }.get(),
        /* flags */ asS32Or(info[2], 0)));
}

Napi::Value end(const Napi::CallbackInfo& info) {
    ImGui::End();
    return info.Env().Undefined();
}

Napi::Value beginChild(const Napi::CallbackInfo& info) {
    if (info[0].IsString()) {
        return fromBool(info.Env(), ImGui::BeginChild(
            /* str_id */ asStrUtf8(info[0]).c_str(),
            /* size */ asVec2Or(info[1], {}),
            /* border */ asBoolOr(info[2], false),
            /* flags */ asS32Or(info[3], 0)));
    }
    else {
        return fromBool(info.Env(), ImGui::BeginChild(
            /* id */ asU32(info[0]),
            /* size */ asVec2Or(info[1], {}),
            /* border */ asBoolOr(info[2], false),
            /* flags */ asS32Or(info[3], 0)));
    }
}

Napi::Value endChild(const Napi::CallbackInfo& info) {
    ImGui::EndChild();
    return info.Env().Undefined();
}

Napi::Value isWindowAppearing(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsWindowAppearing());
}

Napi::Value isWindowCollapsed(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsWindowCollapsed());
}

Napi::Value isWindowFocused(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsWindowFocused(/* flags */ asS32Or(info[0], 0)));
}

Napi::Value isWindowHovered(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsWindowHovered(/* flags */ asS32Or(info[0], 0)));
}

Napi::Value getWindowDrawList(const Napi::CallbackInfo& info) {
    return DrawList::create(info.Env(), ImGui::GetWindowDrawList(), ImGui::GetWindowPos());
}

Napi::Value getWindowDpiScale(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetWindowDpiScale());
}

Napi::Value getWindowPos(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetWindowPos());
}

Napi::Value getWindowSize(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetWindowSize());
}

Napi::Value getWindowWidth(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetWindowWidth());
}

Napi::Value getWindowHeight(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetWindowHeight());
}

Napi::Value getWindowViewport(const Napi::CallbackInfo& info) {
    return Viewport::create(info.Env(), ImGui::GetWindowViewport());
}

Napi::Value setNextWindowPos(const Napi::CallbackInfo& info) {
    ImGui::SetNextWindowPos(
        /* pos */ asVec2(info[0]),
        /* cond */ asS32Or(info[1], 0),
        /* pivot */ asVec2Or(info[2], {}));
    return info.Env().Undefined();
}

Napi::Value setNextWindowSize(const Napi::CallbackInfo& info) {
    ImGui::SetNextWindowSize(
        /* size */ asVec2(info[0]),
        /* cond */ asS32Or(info[1], 0));
    return info.Env().Undefined();
}

void callbackSizeConstraints(ImGuiSizeCallbackData* data) {
    const auto udata = reinterpret_cast<Napi::FunctionReference*>(data->UserData);
    udata->Call({ SizeCallbackData::create(udata->Env(), data) });
}

Napi::Value setNextWindowSizeConstraints(const Napi::CallbackInfo& info) {
    if (info[2].IsUndefined()) {
        ImGui::SetNextWindowSizeConstraints(
            /* size_min */ asVec2(info[0]),
            /* size_max */ asVec2(info[1]));
    }
    else {
        static auto udata = new Napi::FunctionReference;
        *udata = Napi::Persistent(valueAsFunction(info[2]));
        ImGui::SetNextWindowSizeConstraints(
            /* size_min */ asVec2(info[0]),
            /* size_max */ asVec2(info[1]),
            callbackSizeConstraints, udata);
    }
    return info.Env().Undefined();
}

Napi::Value setNextWindowContentSize(const Napi::CallbackInfo& info) {
    ImGui::SetNextWindowContentSize(/* size */ asVec2(info[0]));
    return info.Env().Undefined();
}

Napi::Value setNextWindowCollapsed(const Napi::CallbackInfo& info) {
    ImGui::SetNextWindowCollapsed(
        /* collapsed */ asBool(info[0]),
        /* cond */ asS32Or(info[1], 0));
    return info.Env().Undefined();
}

Napi::Value setNextWindowFocus(const Napi::CallbackInfo& info) {
    ImGui::SetNextWindowFocus();
    return info.Env().Undefined();
}

Napi::Value setNextWindowBgAlpha(const Napi::CallbackInfo& info) {
    ImGui::SetNextWindowBgAlpha(/* alpha */ asF32(info[0]));
    return info.Env().Undefined();
}

Napi::Value setNextWindowViewport(const Napi::CallbackInfo& info) {
    ImGui::SetNextWindowViewport(/* viewport_id */ asU32(info[0]));
    return info.Env().Undefined();
}

Napi::Value setWindowPos(const Napi::CallbackInfo& info) {
    if (info[0].IsString()) {
        ImGui::SetWindowPos(
            /* name */ asStrUtf8(info[0]).c_str(),
            /* pos */ asVec2(info[1]),
            /* cond */ asS32Or(info[2], 0));
    }
    else {
        ImGui::SetWindowPos(
            /* pos */ asVec2(info[0]),
            /* cond */ asS32Or(info[1], 0));
    }
    return info.Env().Undefined();
}

Napi::Value setWindowSize(const Napi::CallbackInfo& info) {
    if (info[0].IsString()) {
        ImGui::SetWindowSize(
            /* name */ asStrUtf8(info[0]).c_str(),
            /* size */ asVec2(info[1]),
            /* cond */ asS32Or(info[2], 0));
    }
    else {
        ImGui::SetWindowSize(
            /* size */ asVec2(info[0]),
            /* cond */ asS32Or(info[1], 0));
    }
    return info.Env().Undefined();
}

Napi::Value setWindowCollapsed(const Napi::CallbackInfo& info) {
    if (info[0].IsString()) {
        ImGui::SetWindowCollapsed(
            /* name */ asStrUtf8(info[0]).c_str(),
            /* collapsed */ asBool(info[1]),
            /* cond */ asS32Or(info[2], 0));
    }
    else {
        ImGui::SetWindowCollapsed(
            /* collapsed */ asBool(info[0]),
            /* cond */ asS32Or(info[1], 0));
    }
    return info.Env().Undefined();
}

Napi::Value setWindowFocus(const Napi::CallbackInfo& info) {
    if (info[0].IsString())
        ImGui::SetWindowFocus(/* name */ asStrUtf8(info[0]).c_str());
    else
        ImGui::SetWindowFocus();
    return info.Env().Undefined();
}

Napi::Value setWindowFontScale(const Napi::CallbackInfo& info) {
    ImGui::SetWindowFontScale(/* scale */ asF32(info[0]));
    return info.Env().Undefined();
}

Napi::Value getContentRegionAvail(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetContentRegionAvail());
}

Napi::Value getContentRegionMax(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetContentRegionMax());
}

Napi::Value getWindowContentRegionMin(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetWindowContentRegionMin());
}

Napi::Value getWindowContentRegionMax(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetWindowContentRegionMax());
}

Napi::Value getScrollX(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetScrollX());
}

Napi::Value getScrollY(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetScrollY());
}

Napi::Value setScrollX(const Napi::CallbackInfo& info) {
    ImGui::SetScrollX(/* scroll_x */ asF32(info[0]));
    return info.Env().Undefined();
}

Napi::Value setScrollY(const Napi::CallbackInfo& info) {
    ImGui::SetScrollY(/* scroll_y */ asF32(info[0]));
    return info.Env().Undefined();
}

Napi::Value getScrollMaxX(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetScrollMaxX());
}

Napi::Value getScrollMaxY(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetScrollMaxY());
}

Napi::Value setScrollHereX(const Napi::CallbackInfo& info) {
    ImGui::SetScrollHereX(/* center_x_ratio */ asF32Or(info[0], 0.5f));
    return info.Env().Undefined();
}

Napi::Value setScrollHereY(const Napi::CallbackInfo& info) {
    ImGui::SetScrollHereY(/* center_y_ratio */ asF32Or(info[0], 0.5f));
    return info.Env().Undefined();
}

Napi::Value setScrollFromPosX(const Napi::CallbackInfo& info) {
    ImGui::SetScrollFromPosX(/* local_x */ asF32(info[0]),
        /* center_x_ratio */ asF32Or(info[1], 0.5f));
    return info.Env().Undefined();
}

Napi::Value setScrollFromPosY(const Napi::CallbackInfo& info) {
    ImGui::SetScrollFromPosY(/* local_y */ asF32(info[0]),
        /* center_y_ratio */ asF32Or(info[1], 0.5f));
    return info.Env().Undefined();
}

Napi::Value pushFont(const Napi::CallbackInfo& info) {
    ImGui::PushFont(/* font */ info[0].IsUndefined() ? nullptr : Font::get(info[0]));
    return info.Env().Undefined();
}

Napi::Value popFont(const Napi::CallbackInfo& info) {
    ImGui::PopFont();
    return info.Env().Undefined();
}

Napi::Value pushStyleColor(const Napi::CallbackInfo& info) {
    ImGui::PushStyleColor(
        /* idx */ asS32(info[0]),
        /* col */ asColor(info[1]).Value);
    return info.Env().Undefined();
}

Napi::Value popStyleColor(const Napi::CallbackInfo& info) {
    ImGui::PopStyleColor(/* count */ asS32Or(info[0], 1));
    return info.Env().Undefined();
}

Napi::Value pushStyleVar(const Napi::CallbackInfo& info) {
    if (info[1].IsNumber()) {
        ImGui::PushStyleVar(
            /* idx */ asS32(info[0]),
            /* val */ asF32(info[1]));
    }
    else {
        ImGui::PushStyleVar(
            /* idx */ asS32(info[0]),
            /* val */ asVec2(info[1]));
    }
    return info.Env().Undefined();
}

Napi::Value popStyleVar(const Napi::CallbackInfo& info) {
    ImGui::PopStyleVar(/* count */ asS32Or(info[0], 1));
    return info.Env().Undefined();
}

Napi::Value pushAllowKeyboardFocus(const Napi::CallbackInfo& info) {
    ImGui::PushAllowKeyboardFocus(/* allow_keyboard_focus */ asBool(info[0]));
    return info.Env().Undefined();
}

Napi::Value popAllowKeyboardFocus(const Napi::CallbackInfo& info) {
    ImGui::PopAllowKeyboardFocus();
    return info.Env().Undefined();
}

Napi::Value pushButtonRepeat(const Napi::CallbackInfo& info) {
    ImGui::PushButtonRepeat(/* repeat */ asBool(info[0]));
    return info.Env().Undefined();
}

Napi::Value popButtonRepeat(const Napi::CallbackInfo& info) {
    ImGui::PopButtonRepeat();
    return info.Env().Undefined();
}

Napi::Value pushItemWidth(const Napi::CallbackInfo& info) {
    ImGui::PushItemWidth(/* item_width */ asF32(info[0]));
    return info.Env().Undefined();
}

Napi::Value popItemWidth(const Napi::CallbackInfo& info) {
    ImGui::PopItemWidth();
    return info.Env().Undefined();
}

Napi::Value setNextItemWidth(const Napi::CallbackInfo& info) {
    ImGui::SetNextItemWidth(/* item_width */ asF32(info[0]));
    return info.Env().Undefined();
}

Napi::Value calcItemWidth(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::CalcItemWidth());
}

Napi::Value pushTextWrapPos(const Napi::CallbackInfo& info) {
    ImGui::PushTextWrapPos(/* wrap_local_pos_x */ asF32Or(info[0], 0.0f));
    return info.Env().Undefined();
}

Napi::Value popTextWrapPos(const Napi::CallbackInfo& info) {
    ImGui::PopTextWrapPos();
    return info.Env().Undefined();
}

Napi::Value getFont(const Napi::CallbackInfo& info) {
    return Font::create(info.Env(), ImGui::GetFont());
}

Napi::Value getFontSize(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetFontSize());
}

Napi::Value getFontTexUvWhitePixel(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetFontTexUvWhitePixel());
}

Napi::Value getStyleColor(const Napi::CallbackInfo& info) {
    return fromColor(info.Env(), ImGui::GetStyleColorVec4(/* idx */ asS32(info[0])));
}

Napi::Value separator(const Napi::CallbackInfo& info) {
    ImGui::Separator();
    return info.Env().Undefined();
}

Napi::Value sameLine(const Napi::CallbackInfo& info) {
    ImGui::SameLine(
        /* offset_from_start_x */ asF32Or(info[0], 0.0f),
        /* spacing */ asF32Or(info[1], -1.0f));
    return info.Env().Undefined();
}

Napi::Value newLine(const Napi::CallbackInfo& info) {
    ImGui::NewLine();
    return info.Env().Undefined();
}

Napi::Value spacing(const Napi::CallbackInfo& info) {
    ImGui::Spacing();
    return info.Env().Undefined();
}

Napi::Value dummy(const Napi::CallbackInfo& info) {
    ImGui::Dummy(/* size */ asVec2(info[0]));
    return info.Env().Undefined();
}

Napi::Value indent(const Napi::CallbackInfo& info) {
    ImGui::Indent(/* indent_w */ asF32Or(info[0], 0.0f));
    return info.Env().Undefined();
}

Napi::Value unindent(const Napi::CallbackInfo& info) {
    ImGui::Unindent(/* indent_w */ asF32Or(info[0], 0.0f));
    return info.Env().Undefined();
}

Napi::Value beginGroup(const Napi::CallbackInfo& info) {
    ImGui::BeginGroup();
    return info.Env().Undefined();
}

Napi::Value endGroup(const Napi::CallbackInfo& info) {
    ImGui::EndGroup();
    return info.Env().Undefined();
}

Napi::Value getCursorPos(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetCursorPos());
}

Napi::Value getCursorPosX(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetCursorPosX());
}

Napi::Value getCursorPosY(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetCursorPosY());
}

Napi::Value setCursorPos(const Napi::CallbackInfo& info) {
    ImGui::SetCursorPos(/* local_pos */ asVec2(info[0]));
    return info.Env().Undefined();
}

Napi::Value setCursorPosX(const Napi::CallbackInfo& info) {
    ImGui::SetCursorPosX(/* local_x */ asF32(info[0]));
    return info.Env().Undefined();
}

Napi::Value setCursorPosY(const Napi::CallbackInfo& info) {
    ImGui::SetCursorPosY(/* local_y */ asF32(info[0]));
    return info.Env().Undefined();
}

Napi::Value getCursorStartPos(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetCursorStartPos());
}

Napi::Value getCursorScreenPos(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetCursorScreenPos());
}

Napi::Value setCursorScreenPos(const Napi::CallbackInfo& info) {
    ImGui::SetCursorScreenPos(/* pos */ asVec2(info[0]));
    return info.Env().Undefined();
}

Napi::Value alignTextToFramePadding(const Napi::CallbackInfo& info) {
    ImGui::AlignTextToFramePadding();
    return info.Env().Undefined();
}

Napi::Value getTextLineHeight(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetTextLineHeight());
}

Napi::Value getTextLineHeightWithSpacing(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetTextLineHeightWithSpacing());
}

Napi::Value getFrameHeight(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetFrameHeight());
}

Napi::Value getFrameHeightWithSpacing(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetFrameHeightWithSpacing());
}

Napi::Value pushId(const Napi::CallbackInfo& info) {
    if (info[0].IsString())
        ImGui::PushID(/* str_id */ asStrUtf8(info[0]).c_str());
    else if (type::isInstanceOf<OpaquePointer>(info[0]))
        ImGui::PushID(/* ptr_id */ OpaquePointer::get(info[0]));
    else
        ImGui::PushID(/* int_id */ asS32(info[0]));
    return info.Env().Undefined();
}

Napi::Value popId(const Napi::CallbackInfo& info) {
    ImGui::PopID();
    return info.Env().Undefined();
}

Napi::Value getId(const Napi::CallbackInfo& info) {
    if (type::isInstanceOf<OpaquePointer>(info[0]))
        return fromU32(info.Env(), ImGui::GetID(/* ptr_id */ OpaquePointer::get(info[0])));
    else
        return fromU32(info.Env(), ImGui::GetID(/* str_id */ asStrUtf8(info[0]).c_str()));
}

Napi::Value text(const Napi::CallbackInfo& info) {
    ImGui::TextUnformatted(/* text */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value textColored(const Napi::CallbackInfo& info) {
    ImGui::TextColored(
        /* col */ asColor(info[0]),
        /* fmt */ "%s", asStrUtf8(info[1]).c_str());
    return info.Env().Undefined();
}

Napi::Value textDisabled(const Napi::CallbackInfo& info) {
    ImGui::TextDisabled(/* fmt */ "%s", asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value textWrapped(const Napi::CallbackInfo& info) {
    ImGui::TextWrapped(/* fmt */ "%s", asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value labelText(const Napi::CallbackInfo& info) {
    ImGui::LabelText(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* fmt */ "%s", asStrUtf8(info[1]).c_str());
    return info.Env().Undefined();
}

Napi::Value bulletText(const Napi::CallbackInfo& info) {
    ImGui::BulletText(/* fmt */ "%s", asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value button(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::Button(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* size */ asVec2Or(info[1], {})));
}

Napi::Value smallButton(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::SmallButton(/* label */ asStrUtf8(info[0]).c_str()));
}

Napi::Value invisibleButton(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::InvisibleButton(
        /* str_id */ asStrUtf8(info[0]).c_str(),
        /* size */ asVec2(info[1]),
        /* flags */ asS32Or(info[2], 0)));
}

Napi::Value arrowButton(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::ArrowButton(
        /* str_id */ asStrUtf8(info[0]).c_str(),
        /* dir */ asS32(info[1])));
}

Napi::Value image(const Napi::CallbackInfo& info) {
    ImGui::Image(
        /* user_texture_id */ OpaquePointer::get(info[0]),
        /* size */ asVec2(info[1]),
        /* uv0 */ asVec2Or(info[2], {}),
        /* uv1 */ asVec2Or(info[3], { 1.0f, 1.0f }),
        /* tint_col */ asColorOr(info[4], IM_COL32_WHITE),
        /* border_col */ asColorOr(info[5], {}));
    return info.Env().Undefined();
}

Napi::Value imageButton(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::ImageButton(
        /* user_texture_id */ OpaquePointer::get(info[0]),
        /* size */ asVec2(info[1]),
        /* uv0 */ asVec2Or(info[2], {}),
        /* uv1 */ asVec2Or(info[3], { 1.0f, 1.0f }),
        /* frame_padding */ asS32Or(info[4], -1),
        /* bg_col */ asColorOr(info[5], {}),
        /* tint_col */ asColorOr(info[6], IM_COL32_WHITE)));
}

Napi::Value checkbox(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::Checkbox(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* v */ BoolRef{ info[1] }.get()));
}

Napi::Value checkboxFlags(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::CheckboxFlags(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* flags */ UintRef{ info[1] }.get(),
        /* flags_value */ asU32(info[2])));
}

Napi::Value radioButton(const Napi::CallbackInfo& info) {
    if (info.Length() == 2) {
        return fromBool(info.Env(), ImGui::RadioButton(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* active */ asBool(info[1])));
    }
    else {
        return fromBool(info.Env(), ImGui::RadioButton(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* v */ IntRef{ info[1] }.get(),
            /* v_button */ asS32(info[2])));
    }
}

Napi::Value progressBar(const Napi::CallbackInfo& info) {
    ImGui::ProgressBar(
        /* fraction */ asF32(info[0]),
        /* size_arg */ asVec2Or(info[1], { -FLT_MIN, 0.0f }),
        /* overlay */ info[2].IsUndefined() ? nullptr : asStrUtf8(info[2]).c_str());
    return info.Env().Undefined();
}

Napi::Value bullet(const Napi::CallbackInfo& info) {
    ImGui::Bullet();
    return info.Env().Undefined();
}

Napi::Value beginCombo(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginCombo(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* preview_value */ asStrUtf8(info[1]).c_str(),
        /* flags */ asS32Or(info[2], 0)));
}

Napi::Value endCombo(const Napi::CallbackInfo& info) {
    ImGui::EndCombo();
    return info.Env().Undefined();
}

bool callbackComboArray(void* data, int idx, const char** out_text) {
    static std::size_t bufSize = 20;
    static auto buf = static_cast<char*>(malloc(bufSize));
    const auto udata = reinterpret_cast<Napi::Reference<Napi::Array>*>(data);
    const auto str = asStrUtf8(udata->Value().Get(idx));

    if (str.size() > bufSize) {
        bufSize *= 2;
        buf = static_cast<char*>(std::realloc(buf, bufSize));
    }
    *out_text = std::strcpy(buf, str.c_str());

    return idx < static_cast<int>(udata->Value().Length());
}

struct callbackComboFunctionData {
    Napi::FunctionReference func;
    int itemsCount;
};

bool callbackComboFunction(void* data, int idx, const char** out_text) {
    static std::size_t bufSize = 20;
    static auto buf = static_cast<char*>(malloc(bufSize));
    const auto udata = reinterpret_cast<callbackComboFunctionData*>(data);
    const auto str = asStrUtf8(udata->func.Call({ fromS32(udata->func.Env(), idx) }));

    if (str.size() > bufSize) {
        bufSize *= 2;
        buf = static_cast<char*>(std::realloc(buf, bufSize));
    }
    *out_text = std::strcpy(buf, str.c_str());

    return idx < udata->itemsCount;
}

Napi::Value combo(const Napi::CallbackInfo& info) {
    if (info[2].IsArray()) {
        static auto udata = new Napi::Reference<Napi::Array>;
        *udata = Napi::Persistent(valueAsArray(info[2]));
        return fromBool(info.Env(), ImGui::Combo(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* current_item */ IntRef{ info[1] }.get(),
            /* items_getter */ callbackComboArray,
            /* data */ udata,
            /* items_count */ udata->Value().Length(),
            /* popup_max_height_in_items */ asS32Or(info[3], -1)));
    }
    else {
        static auto udata = new callbackComboFunctionData;
        udata->func = Napi::Persistent(valueAsFunction(info[2]));
        udata->itemsCount = asS32(info[3]);
        return fromBool(info.Env(), ImGui::Combo(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* current_item */ IntRef{ info[1] }.get(),
            /* items_getter */ callbackComboFunction,
            /* data */ udata,
            /* items_count */ udata->itemsCount,
            /* popup_max_height_in_items */ asS32Or(info[4], -1)));
    }
}

Napi::Value dragNumber(const Napi::CallbackInfo& info) {
    const auto v_min = asF64Or(info[3], 0.0);
    const auto v_max = asF64Or(info[4], 0.0);
    return fromBool(info.Env(), ImGui::DragScalar(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* data_type */ ImGuiDataType_Double,
        /* p_data */ DoubleRef{ info[1] }.get(),
        /* v_speed */ asF32Or(info[2], 1.0f),
        /* p_min */ &v_min,
        /* p_max */ &v_max,
        /* format */ asStrUtf8Or(info[5], "%.3lf").c_str(),
        /* flags */ asS32Or(info[6], 0)));
}

Napi::Value dragNumbers(const Napi::CallbackInfo& info) {
    const auto data = valueAsArray(info[1]);
    std::vector<double> doubles(data.Length());
    for (std::size_t i{}; i < doubles.size(); ++i) doubles[i] = asF64(data.Get(i));

    const auto v_min = asF64Or(info[3], 0.0);
    const auto v_max = asF64Or(info[4], 0.0);
    const auto ret = ImGui::DragScalarN(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* data_type */ ImGuiDataType_Double,
        /* p_data */ doubles.data(),
        /* components */ doubles.size(),
        /* v_speed */ asF32Or(info[2], 1.0f),
        /* p_min */ &v_min,
        /* p_max */ &v_max,
        /* format */ asStrUtf8Or(info[5], "%.3lf").c_str(),
        /* flags */ asS32Or(info[6], 0));

    for (std::size_t i{}; i < doubles.size(); ++i) data.Set(i, fromF64(info.Env(), doubles[i]));
    return fromBool(info.Env(), ret);
}

Napi::Value dragNumberRange(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::DragFloatRange2(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* v_current_min */ FloatRef{ info[1] }.get(),
        /* v_current_max */ FloatRef{ info[2] }.get(),
        /* v_speed */ asF32Or(info[3], 1.0f),
        /* v_min */ asF32Or(info[4], 0.0f),
        /* v_max */ asF32Or(info[5], 0.0f),
        /* format */ asStrUtf8Or(info[6], "%.3lf").c_str(),
        /* format_max */ info[7].IsUndefined() ? nullptr : asStrUtf8(info[7]).c_str(),
        /* flags */ asS32Or(info[8], 0)));
}

Napi::Value sliderNumber(const Napi::CallbackInfo& info) {
    const auto v_min = asF64Or(info[2], 0.0);
    const auto v_max = asF64Or(info[3], 0.0);
    return fromBool(info.Env(), ImGui::SliderScalar(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* data_type */ ImGuiDataType_Double,
        /* p_data */ DoubleRef{ info[1] }.get(),
        /* p_min */ &v_min,
        /* p_max */ &v_max,
        /* format */ asStrUtf8Or(info[4], "%.3lf").c_str(),
        /* flags */ asS32Or(info[5], 0)));
}

Napi::Value sliderNumbers(const Napi::CallbackInfo& info) {
    const auto data = valueAsArray(info[1]);
    std::vector<double> doubles(data.Length());
    for (std::size_t i{}; i < doubles.size(); ++i) doubles[i] = asF64(data.Get(i));

    const auto v_min = asF64Or(info[2], 0.0);
    const auto v_max = asF64Or(info[3], 0.0);
    const auto ret = ImGui::SliderScalarN(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* data_type */ ImGuiDataType_Double,
        /* p_data */ doubles.data(),
        /* components */ doubles.size(),
        /* p_min */ &v_min,
        /* p_max */ &v_max,
        /* format */ asStrUtf8Or(info[4], "%.3lf").c_str(),
        /* flags */ asS32Or(info[5], 0));

    for (std::size_t i{}; i < doubles.size(); ++i) data.Set(i, fromF64(info.Env(), doubles[i]));
    return fromBool(info.Env(), ret);
}

Napi::Value vSliderNumber(const Napi::CallbackInfo& info) {
    const auto v_min = asF64Or(info[3], 0.0);
    const auto v_max = asF64Or(info[4], 0.0);
    return fromBool(info.Env(), ImGui::VSliderScalar(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* size */ asVec2(info[1]),
        /* data_type */ ImGuiDataType_Double,
        /* p_data */ DoubleRef{ info[2] }.get(),
        /* p_min */ &v_min,
        /* p_max */ &v_max,
        /* format */ asStrUtf8Or(info[5], "%.3lf").c_str(),
        /* flags */ asS32Or(info[6], 0)));
}

Napi::Value sliderAngle(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::SliderAngle(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* v_rad */ FloatRef{ info[1] }.get(),
        /* v_degrees_min */ asF32Or(info[2], -360.0f),
        /* v_degrees_max */ asF32Or(info[3], +360.0f),
        /* format */ asStrUtf8Or(info[4], "%.0f deg").c_str(),
        /* flags */ asS32Or(info[5], 0)));
}

int callbackInputText(ImGuiInputTextCallbackData* data) {
    const auto udata = reinterpret_cast<Napi::FunctionReference*>(data->UserData);
    return asS32(udata->Call({ InputTextCallbackData::create(udata->Env(), data) }));
}

Napi::Value inputText(const Napi::CallbackInfo& info) {
    if (info[3].IsUndefined()) {
        return fromBool(info.Env(), ImGui::InputText(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* str */ StringRef{ info[1] }.get(),
            /* flags */ asS32Or(info[2], 0)));
    }
    else {
        static auto udata = new Napi::FunctionReference;
        *udata = Napi::Persistent(valueAsFunction(info[3]));
        return fromBool(info.Env(), ImGui::InputText(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* str */ StringRef{ info[1] }.get(),
            /* flags */ asS32Or(info[2], 0),
            /* callback */ callbackInputText, udata));
    }
}

Napi::Value inputTextMultiline(const Napi::CallbackInfo& info) {
    if (info[4].IsUndefined()) {
        return fromBool(info.Env(), ImGui::InputTextMultiline(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* str */ StringRef{ info[1] }.get(),
            /* size */ asVec2Or(info[2], {}),
            /* flags */ asS32Or(info[3], 0)));
    }
    else {
        static auto udata = new Napi::FunctionReference;
        *udata = Napi::Persistent(valueAsFunction(info[4]));
        return fromBool(info.Env(), ImGui::InputTextMultiline(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* str */ StringRef{ info[1] }.get(),
            /* size */ asVec2Or(info[2], {}),
            /* flags */ asS32Or(info[3], 0),
            /* callback */ callbackInputText, udata));
    }
}

Napi::Value inputTextWithHint(const Napi::CallbackInfo& info) {
    if (info[4].IsUndefined()) {
        return fromBool(info.Env(), ImGui::InputTextWithHint(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* hint */ asStrUtf8(info[1]).c_str(),
            /* str */ StringRef{ info[2] }.get(),
            /* flags */ asS32Or(info[3], 0)));
    }
    else {
        static auto udata = new Napi::FunctionReference;
        *udata = Napi::Persistent(valueAsFunction(info[4]));
        return fromBool(info.Env(), ImGui::InputTextWithHint(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* hint */ asStrUtf8(info[1]).c_str(),
            /* str */ StringRef{ info[2] }.get(),
            /* flags */ asS32Or(info[3], 0),
            /* callback */ callbackInputText, udata));
    }
}

Napi::Value inputNumber(const Napi::CallbackInfo& info) {
    const auto step = asF64Or(info[2], 1.0);
    const auto stepFast = asF64Or(info[3], 100.0);
    return fromBool(info.Env(), ImGui::InputScalar(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* data_type */ ImGuiDataType_Double,
        /* p_data */ DoubleRef{ info[1] }.get(),
        /* p_step */ &step,
        /* p_step_max */ &stepFast,
        /* format */ asStrUtf8Or(info[4], "%.3lf").c_str(),
        /* flags */ asS32Or(info[5], 0)));
}

Napi::Value inputNumbers(const Napi::CallbackInfo& info) {
    const auto data = valueAsArray(info[1]);
    std::vector<double> doubles(data.Length());
    for (std::size_t i{}; i < doubles.size(); ++i) doubles[i] = asF64(data.Get(i));

    const auto step = asF64Or(info[2], 1.0);
    const auto stepFast = asF64Or(info[3], 100.0);
    const auto ret = ImGui::InputScalarN(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* data_type */ ImGuiDataType_Double,
        /* p_data */ doubles.data(),
        /* components */ doubles.size(),
        /* p_step */ &step,
        /* p_step_fast */ &stepFast,
        /* format */ asStrUtf8Or(info[4], "%.3lf").c_str(),
        /* flags */ asS32Or(info[5], 0));

    for (std::size_t i{}; i < doubles.size(); ++i) data.Set(i, fromF64(info.Env(), doubles[i]));
    return fromBool(info.Env(), ret);
}

Napi::Value colorEdit3(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::ColorEdit3(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* col */ ColorRef{ info[1] }.get(),
        /* flags */ asS32Or(info[2], 0)));
}

Napi::Value colorEdit4(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::ColorEdit4(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* col */ ColorRef{ info[1] }.get(),
        /* flags */ asS32Or(info[2], 0)));
}

Napi::Value colorPicker3(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::ColorPicker3(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* col */ ColorRef{ info[1] }.get(),
        /* flags */ asS32Or(info[2], 0)));
}

Napi::Value colorPicker4(const Napi::CallbackInfo& info) {
    const auto refCol = info[3].IsUndefined() ? std::nullopt : std::optional{ asColor(info[3]) };
    return fromBool(info.Env(), ImGui::ColorPicker4(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* col */ ColorRef{ info[1] }.get(),
        /* flags */ asS32Or(info[2], 0),
        /* ref_col */ refCol.has_value() ? &refCol.value().Value.x : nullptr));
}

Napi::Value colorButton(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::ColorButton(
        /* desc_id */ asStrUtf8(info[0]).c_str(),
        /* col */ asColor(info[1]),
        /* flags */ asS32Or(info[2], 0),
        /* size */ asVec2Or(info[3], {})));
}

Napi::Value setColorEditOptions(const Napi::CallbackInfo& info) {
    ImGui::SetColorEditOptions(asS32(info[0]));
    return info.Env().Undefined();
}

Napi::Value treeNode(const Napi::CallbackInfo& info) {
    if (info.Length() == 1) {
        return fromBool(info.Env(), ImGui::TreeNode(
            /* label */ asStrUtf8(info[0]).c_str()));
    }
    else {
        if (info[0].IsString()) {
            return fromBool(info.Env(), ImGui::TreeNode(
                /* str_id */ asStrUtf8(info[0]).c_str(),
                /* fmt */ "%s", asStrUtf8(info[1]).c_str()));
        }
        else {
            return fromBool(info.Env(), ImGui::TreeNode(
                /* ptr_id */ OpaquePointer::get(info[0]),
                /* fmt */ "%s", asStrUtf8(info[1]).c_str()));
        }
    }
}

Napi::Value treeNodeEx(const Napi::CallbackInfo& info) {
    if (info.Length() == 1) {
        return fromBool(info.Env(), ImGui::TreeNodeEx(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* flags */ asS32Or(info[1], 0)));
    }
    else {
        if (info[0].IsString()) {
            return fromBool(info.Env(), ImGui::TreeNodeEx(
                /* str_id */ asStrUtf8(info[0]).c_str(),
                /* flags */ asS32(info[1]),
                /* fmt */ "%s", asStrUtf8(info[2]).c_str()));
        }
        else {
            return fromBool(info.Env(), ImGui::TreeNodeEx(
                /* ptr_id */ OpaquePointer::get(info[0]),
                /* flags */ asS32(info[1]),
                /* fmt */ "%s", asStrUtf8(info[2]).c_str()));
        }
    }
}

Napi::Value treePush(const Napi::CallbackInfo& info) {
    if (info.Length() == 1 && info[0].IsString())
        ImGui::TreePush(/* str_id */ asStrUtf8(info[0]).c_str());
    else
        ImGui::TreePush(/* ptr_id */ info[0].IsUndefined() ? nullptr : OpaquePointer::get(info[0]));
    return info.Env().Undefined();
}

Napi::Value treePop(const Napi::CallbackInfo& info) {
    ImGui::TreePop();
    return info.Env().Undefined();
}

Napi::Value getTreeNodeToLabelSpacing(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetTreeNodeToLabelSpacing());
}

Napi::Value collapsingHeader(const Napi::CallbackInfo& info) {
    if (info[1].IsArray()) {
        return fromBool(info.Env(), ImGui::CollapsingHeader(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* p_visible */ BoolRef{ info[1] }.get(),
            /* flags */ asS32Or(info[2], 0)));
    }
    else {
        return fromBool(info.Env(), ImGui::CollapsingHeader(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* flags */ asS32Or(info[1], 0)));
    }
}

Napi::Value setNextItemOpen(const Napi::CallbackInfo& info) {
    ImGui::SetNextItemOpen(
        /* is_open */ asBool(info[0]),
        /* cond */ asS32Or(info[1], 0));
    return info.Env().Undefined();
}

Napi::Value selectable(const Napi::CallbackInfo& info) {
    if (info[1].IsArray()) {
        return fromBool(info.Env(), ImGui::Selectable(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* p_selected */ BoolRef{ info[1] }.get(),
            /* flags */ asS32Or(info[2], 0),
            /* size */ asVec2Or(info[3], {})));
    }
    else {
        return fromBool(info.Env(), ImGui::Selectable(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* selected */ asBoolOr(info[1], false),
            /* flags */ asS32Or(info[2], 0),
            /* size */ asVec2Or(info[3], {})));
    }
}

Napi::Value beginListBox(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginListBox(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* size */ asVec2Or(info[1], {})));
}

Napi::Value endListBox(const Napi::CallbackInfo& info) {
    ImGui::EndListBox();
    return info.Env().Undefined();
}

Napi::Value listBox(const Napi::CallbackInfo& info) {
    if (info[2].IsArray()) {
        static auto udata = new Napi::Reference<Napi::Array>;
        *udata = Napi::Persistent(valueAsArray(info[2]));
        return fromBool(info.Env(), ImGui::ListBox(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* current_item */ IntRef{ info[1] }.get(),
            /* items_getter */ callbackComboArray,
            /* data */ udata,
            /* items_count */ udata->Value().Length(),
            /* height_in_items */ asS32Or(info[3], -1)));
    }
    else {
        static auto udata = new callbackComboFunctionData;
        udata->func = Napi::Persistent(valueAsFunction(info[2]));
        udata->itemsCount = asS32(info[3]);
        return fromBool(info.Env(), ImGui::ListBox(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* current_item */ IntRef{ info[1] }.get(),
            /* items_getter */ callbackComboFunction,
            /* data */ udata,
            /* items_count */ udata->itemsCount,
            /* height_in_items */ asS32Or(info[4], -1)));
    }
}

float callbackPlotArray(void* data, int idx) {
    const auto udata = reinterpret_cast<Napi::Reference<Napi::Array>*>(data);
    return asF32(udata->Value().Get(idx));
}

float callbackPlotFunction(void* data, int idx) {
    const auto udata = reinterpret_cast<Napi::FunctionReference*>(data);
    return asF32(udata->Call({ fromS32(udata->Env(), idx) }));
}

Napi::Value plotLines(const Napi::CallbackInfo& info) {
    if (info[1].IsArray()) {
        static auto udata = new Napi::Reference<Napi::Array>;
        *udata = Napi::Persistent(valueAsArray(info[1]));
        ImGui::PlotLines(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* values_getter */ callbackPlotArray,
            /* data */ udata,
            /* values_count */ udata->Value().Length(),
            /* values_offset */ 0,
            /* overlay_text */ info[2].IsUndefined() ? nullptr : asStrUtf8(info[2]).c_str(),
            /* scale_min */ asF32Or(info[3], FLT_MAX),
            /* scale_max */ asF32Or(info[4], FLT_MAX),
            /* graph_size */ asVec2Or(info[5], {}));
    }
    else {
        static auto udata = new Napi::FunctionReference;
        *udata = Napi::Persistent(valueAsFunction(info[1]));
        ImGui::PlotLines(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* values_getter */ callbackPlotFunction,
            /* data */ udata,
            /* values_count */ asS32(info[2]),
            /* values_offset */ 0,
            /* overlay_text */ info[3].IsUndefined() ? nullptr : asStrUtf8(info[3]).c_str(),
            /* scale_min */ asF32Or(info[4], FLT_MAX),
            /* scale_max */ asF32Or(info[5], FLT_MAX),
            /* graph_size */ asVec2Or(info[6], {}));
    }
    return info.Env().Undefined();
}

Napi::Value plotHistogram(const Napi::CallbackInfo& info) {
    if (info[1].IsArray()) {
        static auto udata = new Napi::Reference<Napi::Array>;
        *udata = Napi::Persistent(valueAsArray(info[1]));
        ImGui::PlotHistogram(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* values_getter */ callbackPlotArray,
            /* data */ udata,
            /* values_count */ udata->Value().Length(),
            /* values_offset */ 0,
            /* overlay_text */ info[2].IsUndefined() ? nullptr : asStrUtf8(info[2]).c_str(),
            /* scale_min */ asF32Or(info[3], FLT_MAX),
            /* scale_max */ asF32Or(info[4], FLT_MAX),
            /* graph_size */ asVec2Or(info[5], {}));
    }
    else {
        static auto udata = new Napi::FunctionReference;
        *udata = Napi::Persistent(valueAsFunction(info[1]));
        ImGui::PlotHistogram(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* values_getter */ callbackPlotFunction,
            /* data */ udata,
            /* values_count */ asS32(info[2]),
            /* values_offset */ 0,
            /* overlay_text */ info[3].IsUndefined() ? nullptr : asStrUtf8(info[3]).c_str(),
            /* scale_min */ asF32Or(info[4], FLT_MAX),
            /* scale_max */ asF32Or(info[5], FLT_MAX),
            /* graph_size */ asVec2Or(info[6], {}));
    }
    return info.Env().Undefined();
}

Napi::Value value(const Napi::CallbackInfo& info) {
    if (info[1].IsBoolean()) {
        ImGui::Value(
            /* prefix */ asStrUtf8(info[0]).c_str(),
            /* b */ asBool(info[1]));
    }
    else {
        ImGui::Value(
            /* prefix */ asStrUtf8(info[0]).c_str(),
            /* v */ asF32(info[1]),
            /* float_format */ info[2].IsUndefined() ? nullptr : asStrUtf8(info[2]).c_str());
    }
    return info.Env().Undefined();
}

Napi::Value beginMenuBar(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginMenuBar());
}

Napi::Value endMenuBar(const Napi::CallbackInfo& info) {
    ImGui::EndMenuBar();
    return info.Env().Undefined();
}

Napi::Value beginMainMenuBar(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginMainMenuBar());
}

Napi::Value endMainMenuBar(const Napi::CallbackInfo& info) {
    ImGui::EndMainMenuBar();
    return info.Env().Undefined();
}

Napi::Value beginMenu(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginMenu(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* enabled */ asBoolOr(info[1], true)));
}

Napi::Value endMenu(const Napi::CallbackInfo& info) {
    ImGui::EndMenu();
    return info.Env().Undefined();
}

Napi::Value menuItem(const Napi::CallbackInfo& info) {
    if (info.Length() == 1 || !info[2].IsArray()) {
        return fromBool(info.Env(), ImGui::MenuItem(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* shortcut */ info[1].IsUndefined() ? nullptr : asStrUtf8(info[1]).c_str(),
            /* selected */ asBoolOr(info[2], false),
            /* enabled */ asBoolOr(info[3], true)));
    }
    else {
        return fromBool(info.Env(), ImGui::MenuItem(
            /* label */ asStrUtf8(info[0]).c_str(),
            /* shortcut */ asStrUtf8(info[1]).c_str(),
            /* p_selected */ BoolRef{ info[2] }.get(),
            /* enabled */ asBoolOr(info[3], true)));
    }
}

Napi::Value beginTooltip(const Napi::CallbackInfo& info) {
    ImGui::BeginTooltip();
    return info.Env().Undefined();
}

Napi::Value endTooltip(const Napi::CallbackInfo& info) {
    ImGui::EndTooltip();
    return info.Env().Undefined();
}

Napi::Value setTooltip(const Napi::CallbackInfo& info) {
    ImGui::SetTooltip(/* fmt */ "%s", asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value beginPopup(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginPopup(
        /* str_id */ asStrUtf8(info[0]).c_str(),
        /* flags */ asS32Or(info[1], 0)));
}

Napi::Value beginPopupModal(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginPopupModal(
        /* name */ asStrUtf8(info[0]).c_str(),
        /* p_open */ info[1].IsUndefined() ? nullptr : BoolRef{ info[1] }.get(),
        /* flags */ asS32Or(info[2], 0)));
}

Napi::Value endPopup(const Napi::CallbackInfo& info) {
    ImGui::EndPopup();
    return info.Env().Undefined();
}

Napi::Value openPopup(const Napi::CallbackInfo& info) {
    if (info[0].IsString()) {
        ImGui::OpenPopup(
            /* str_id */ asStrUtf8(info[0]).c_str(),
            /* popup_flags */ asS32Or(info[1], 0));
    }
    else {
        ImGui::OpenPopup(
            /* id */ asU32(info[0]),
            /* popup_flags */ asS32Or(info[1], 0));
    }
    return info.Env().Undefined();
}

Napi::Value openPopupOnItemClick(const Napi::CallbackInfo& info) {
    ImGui::OpenPopupOnItemClick(
        /* str_id */ info[0].IsUndefined() ? nullptr : asStrUtf8(info[0]).c_str(),
        /* popup_flags */ asS32Or(info[1], 1));
    return info.Env().Undefined();
}

Napi::Value closeCurrentPopup(const Napi::CallbackInfo& info) {
    ImGui::CloseCurrentPopup();
    return info.Env().Undefined();
}

Napi::Value beginPopupContextItem(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginPopupContextItem(
        /* str_id */ info[0].IsUndefined() ? nullptr : asStrUtf8(info[0]).c_str(),
        /* popup_flags */ asS32Or(info[1], 1)));
}

Napi::Value beginPopupContextWindow(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginPopupContextWindow(
        /* str_id */ info[0].IsUndefined() ? nullptr : asStrUtf8(info[0]).c_str(),
        /* popup_flags */ asS32Or(info[1], 1)));
}

Napi::Value beginPopupContextVoid(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginPopupContextVoid(
        /* str_id */ info[0].IsUndefined() ? nullptr : asStrUtf8(info[0]).c_str(),
        /* popup_flags */ asS32Or(info[1], 1)));
}

Napi::Value isPopupOpen(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsPopupOpen(
        /* str_id */ asStrUtf8(info[0]).c_str(),
        /* flags */ asS32Or(info[1], 0)));
}

Napi::Value beginTable(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginTable(
        /* str_id */ asStrUtf8(info[0]).c_str(),
        /* column */ asS32(info[1]),
        /* flags */ asS32Or(info[2], 0),
        /* outer_size */ asVec2Or(info[3], {}),
        /* inner_width */ asF32Or(info[4], 0.0f)));
}

Napi::Value endTable(const Napi::CallbackInfo& info) {
    ImGui::EndTable();
    return info.Env().Undefined();
}

Napi::Value tableNextRow(const Napi::CallbackInfo& info) {
    ImGui::TableNextRow(
        /* row_flags */ asS32Or(info[0], 0),
        /* min_row_height */ asF32Or(info[1], 0.0f));
    return info.Env().Undefined();
}

Napi::Value tableNextColumn(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::TableNextColumn());
}

Napi::Value tableSetColumnIndex(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::TableSetColumnIndex(/* column_n */ asS32(info[0])));
}

Napi::Value tableSetupColumn(const Napi::CallbackInfo& info) {
    ImGui::TableSetupColumn(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* flags */ asS32Or(info[1], 0),
        /* init_width_or_weight */ asF32Or(info[2], 0.0f),
        /* user_id */ asU32Or(info[3], 0));
    return info.Env().Undefined();
}

Napi::Value tableSetupScrollFreeze(const Napi::CallbackInfo& info) {
    ImGui::TableSetupScrollFreeze(
        /* cols */ asS32(info[0]),
        /* rows */ asS32(info[1]));
    return info.Env().Undefined();
}

Napi::Value tableHeadersRow(const Napi::CallbackInfo& info) {
    ImGui::TableHeadersRow();
    return info.Env().Undefined();
}

Napi::Value tableHeader(const Napi::CallbackInfo& info) {
    ImGui::TableHeader(/* label */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value tableGetSortSpecs(const Napi::CallbackInfo& info) {
    const auto sortSpecs = ImGui::TableGetSortSpecs();
    return sortSpecs ? TableSortSpecs::create(info.Env(), sortSpecs) : info.Env().Undefined();
}

Napi::Value tableGetColumnCount(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::TableGetColumnCount());
}

Napi::Value tableGetColumnIndex(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::TableGetColumnIndex());
}

Napi::Value tableGetRowIndex(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::TableGetRowIndex());
}

Napi::Value tableGetColumnName(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), ImGui::TableGetColumnName(/* column_n */ asS32Or(info[0], -1)));
}

Napi::Value tableGetColumnFlags(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::TableGetColumnFlags(/* column_n */ asS32Or(info[0], -1)));
}

Napi::Value tableSetColumnEnabled(const Napi::CallbackInfo& info) {
    ImGui::TableSetColumnEnabled(
        /* column_n */ asS32(info[0]),
        /* v */ asBool(info[1]));
    return info.Env().Undefined();
}

Napi::Value tableSetBgColor(const Napi::CallbackInfo& info) {
    ImGui::TableSetBgColor(
        /* target */ asS32(info[0]),
        /* color */ asColor(info[1]),
        /* column_n */ asS32Or(info[2], -1));
    return info.Env().Undefined();
}

Napi::Value columns(const Napi::CallbackInfo& info) {
    ImGui::Columns(
        /* count */ asS32Or(info[0], 1),
        /* id */ info[1].IsUndefined() ? nullptr : asStrUtf8(info[1]).c_str(),
        /* border */ asBoolOr(info[2], true));
    return info.Env().Undefined();
}

Napi::Value nextColumn(const Napi::CallbackInfo& info) {
    ImGui::NextColumn();
    return info.Env().Undefined();
}

Napi::Value getColumnIndex(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::GetColumnIndex());
}

Napi::Value getColumnWidth(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetColumnWidth(/* column_index */ asS32Or(info[0], -1)));
}

Napi::Value setColumnWidth(const Napi::CallbackInfo& info) {
    ImGui::SetColumnWidth(
        /* column_index */ asS32(info[0]),
        /* width */ asF32(info[1]));
    return info.Env().Undefined();
}

Napi::Value getColumnOffset(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), ImGui::GetColumnOffset(/* column_index */ asS32Or(info[0], -1)));
}

Napi::Value setColumnOffset(const Napi::CallbackInfo& info) {
    ImGui::SetColumnOffset(
        /* column_index */ asS32(info[0]),
        /* offset_x */ asF32(info[1]));
    return info.Env().Undefined();
}

Napi::Value getColumnsCount(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::GetColumnsCount());
}

Napi::Value beginTabBar(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginTabBar(
        /* str_id */ asStrUtf8(info[0]).c_str(),
        /* flags */ asS32Or(info[1], 0)));
}

Napi::Value endTabBar(const Napi::CallbackInfo& info) {
    ImGui::EndTabBar();
    return info.Env().Undefined();
}

Napi::Value beginTabItem(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginTabItem(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* p_open */ info[1].IsUndefined() ? nullptr : BoolRef{ info[1] }.get(),
        /* flags */ asS32Or(info[2], 0)));
}

Napi::Value endTabItem(const Napi::CallbackInfo& info) {
    ImGui::EndTabItem();
    return info.Env().Undefined();
}

Napi::Value tabItemButton(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::TabItemButton(
        /* label */ asStrUtf8(info[0]).c_str(),
        /* flags */ asS32Or(info[1], 0)));
}

Napi::Value setTabItemClosed(const Napi::CallbackInfo& info) {
    ImGui::SetTabItemClosed(/* tab_or_docked_window_label */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value dockSpace(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), ImGui::DockSpace(
        /* id */ asU32(info[0]),
        /* size */ asVec2Or(info[1], {}),
        /* flags */ asS32Or(info[2], 0),
        /* window_class */ info[3].IsUndefined() ? nullptr : WindowClass::get(info[3])));
}

Napi::Value dockSpaceOverViewport(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), ImGui::DockSpaceOverViewport(
        /* viewport */ info[0].IsUndefined() ? nullptr : Viewport::get(info[0]),
        /* flags */ asS32Or(info[1], 0),
        /* window_class */ info[2].IsUndefined() ? nullptr : WindowClass::get(info[2])));
}

Napi::Value setNextWindowDockId(const Napi::CallbackInfo& info) {
    ImGui::SetNextWindowDockID(
        /* dock_id */ asU32(info[0]),
        /* cond */ asS32Or(info[1], 0));
    return info.Env().Undefined();
}

Napi::Value setNextWindowClass(const Napi::CallbackInfo& info) {
    ImGui::SetNextWindowClass(/* window_class */ WindowClass::get(info[0]));
    return info.Env().Undefined();
}

Napi::Value getWindowDockId(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), ImGui::GetWindowDockID());
}

Napi::Value isWindowDocked(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsWindowDocked());
}

Napi::Value logToTTY(const Napi::CallbackInfo& info) {
    ImGui::LogToTTY(/* auto_open_depth */ asS32Or(info[0], -1));
    return info.Env().Undefined();
}

Napi::Value logToFile(const Napi::CallbackInfo& info) {
    ImGui::LogToFile(
        /* auto_open_depth */ asS32Or(info[0], -1),
        /* filename */ info[1].IsUndefined() ? nullptr : asStrUtf8(info[1]).c_str());
    return info.Env().Undefined();
}

Napi::Value logToClipboard(const Napi::CallbackInfo& info) {
    ImGui::LogToClipboard(/* auto_open_depth */ asS32Or(info[0], -1));
    return info.Env().Undefined();
}

Napi::Value logFinish(const Napi::CallbackInfo& info) {
    ImGui::LogFinish();
    return info.Env().Undefined();
}

Napi::Value logButtons(const Napi::CallbackInfo& info) {
    ImGui::LogButtons();
    return info.Env().Undefined();
}

Napi::Value logText(const Napi::CallbackInfo& info) {
    ImGui::LogText(/* fmt */ "%s", asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value beginDragDropSource(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginDragDropSource(/* flags */ asS32Or(info[0], 0)));
}

Napi::Value setDragDropPayload(const Napi::CallbackInfo& info) {
    auto data = info[1].As<Napi::Uint8Array>();
    return fromBool(info.Env(), ImGui::SetDragDropPayload(
        /* type */ asStrUtf8(info[0]).c_str(),
        /* data */ data.Data(),
        /* sz */ data.ByteLength(),
        /* cond */ asS32Or(info[2], 0)));
}

Napi::Value endDragDropSource(const Napi::CallbackInfo& info) {
    ImGui::EndDragDropSource();
    return info.Env().Undefined();
}

Napi::Value beginDragDropTarget(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginDragDropTarget());
}

Napi::Value acceptDragDropPayload(const Napi::CallbackInfo& info) {
    return Payload::create(info.Env(), const_cast<ImGuiPayload*>(ImGui::AcceptDragDropPayload(
        /* type */ asStrUtf8(info[0]).c_str(),
        /* flags */ asS32Or(info[1], 0))));
}

Napi::Value endDragDropTarget(const Napi::CallbackInfo& info) {
    ImGui::EndDragDropTarget();
    return info.Env().Undefined();
}

Napi::Value getDragDropPayload(const Napi::CallbackInfo& info) {
    return Payload::create(info.Env(), const_cast<ImGuiPayload*>(ImGui::GetDragDropPayload()));
}

Napi::Value beginDisabled(const Napi::CallbackInfo& info) {
    ImGui::BeginDisabled(/* disabled */ asBoolOr(info[0], true));
    return info.Env().Undefined();
}

Napi::Value endDisabled(const Napi::CallbackInfo& info) {
    ImGui::EndDisabled();
    return info.Env().Undefined();
}

Napi::Value pushClipRect(const Napi::CallbackInfo& info) {
    ImGui::PushClipRect(
        /* clip_rect_min */ asVec2(info[0]),
        /* clip_rect_max */ asVec2(info[1]),
        /* intersect_with_current_clip_rect */ asBool(info[2]));
    return info.Env().Undefined();
}

Napi::Value popClipRect(const Napi::CallbackInfo& info) {
    ImGui::PopClipRect();
    return info.Env().Undefined();
}

Napi::Value setItemDefaultFocus(const Napi::CallbackInfo& info) {
    ImGui::SetItemDefaultFocus();
    return info.Env().Undefined();
}

Napi::Value setKeyboardFocusHere(const Napi::CallbackInfo& info) {
    ImGui::SetKeyboardFocusHere(/* offset */ asS32Or(info[0], 0));
    return info.Env().Undefined();
}

Napi::Value isItemHovered(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemHovered(/* flags */ asS32Or(info[0], 0)));
}

Napi::Value isItemActive(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemActive());
}

Napi::Value isItemFocused(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemFocused());
}

Napi::Value isItemClicked(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemClicked(/* flags */ asS32Or(info[0], 0)));
}

Napi::Value isItemVisible(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemVisible());
}

Napi::Value isItemEdited(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemEdited());
}

Napi::Value isItemActivated(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemActivated());
}

Napi::Value isItemDeactivated(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemDeactivated());
}

Napi::Value isItemDeactivatedAfterEdit(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemDeactivatedAfterEdit());
}

Napi::Value isItemToggledOpen(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsItemToggledOpen());
}

Napi::Value isAnyItemHovered(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsAnyItemHovered());
}

Napi::Value isAnyItemActive(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsAnyItemActive());
}

Napi::Value isAnyItemFocused(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsAnyItemFocused());
}

Napi::Value getItemRectMin(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetItemRectMin());
}

Napi::Value getItemRectMax(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetItemRectMax());
}

Napi::Value getItemRectSize(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetItemRectSize());
}

Napi::Value setItemAllowOverlap(const Napi::CallbackInfo& info) {
    ImGui::SetItemAllowOverlap();
    return info.Env().Undefined();
}

Napi::Value getMainViewport(const Napi::CallbackInfo& info) {
    return Viewport::create(info.Env(), ImGui::GetMainViewport());
}

Napi::Value getBackgroundDrawList(const Napi::CallbackInfo& info) {
    if (info.Length() == 0) {
        return DrawList::create(info.Env(), ImGui::GetBackgroundDrawList(),
            ImGui::GetWindowViewport()->WorkPos);
    }
    else {
        const auto viewport = Viewport::get(info[0]);
        return DrawList::create(info.Env(), ImGui::GetBackgroundDrawList(viewport),
            viewport->WorkPos);
    }
}

Napi::Value getForegroundDrawList(const Napi::CallbackInfo& info) {
    if (info.Length() == 0) {
        return DrawList::create(info.Env(), ImGui::GetForegroundDrawList(),
            ImGui::GetWindowViewport()->WorkPos);
    }
    else {
        const auto viewport = Viewport::get(info[0]);
        return DrawList::create(info.Env(), ImGui::GetForegroundDrawList(viewport),
            viewport->WorkPos);
    }
}

Napi::Value isRectVisible(const Napi::CallbackInfo& info) {
    if (info.Length() == 1) {
        return fromBool(info.Env(), ImGui::IsRectVisible(/* size */ asVec2(info[0])));
    }
    else {
        return fromBool(info.Env(), ImGui::IsRectVisible(
            /* rect_min */ asVec2(info[0]),
            /* rect_max */ asVec2(info[1])));
    }
}

Napi::Value getTime(const Napi::CallbackInfo& info) {
    return fromF64(info.Env(), ImGui::GetTime());
}

Napi::Value getFrameCount(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::GetFrameCount());
}

Napi::Value getStyleColorName(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), ImGui::GetStyleColorName(/* idx */ asS32(info[0])));
}

Napi::Value beginChildFrame(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::BeginChildFrame(
        /* id */ asU32(info[0]),
        /* size */ asVec2(info[1]),
        /* flags */ asS32Or(info[2], 0)));
}

Napi::Value endChildFrame(const Napi::CallbackInfo& info) {
    ImGui::EndChildFrame();
    return info.Env().Undefined();
}

Napi::Value calcTextSize(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::CalcTextSize(
        /* text */ asStrUtf8(info[0]).c_str(), nullptr,
        /* hide_text_after_double_hash */ asBoolOr(info[1], false),
        /* wrap_width */ asF32Or(info[2], -1.0f)));
}

Napi::Value colorConvertRGBtoHSV(const Napi::CallbackInfo& info) {
    const auto inObj = valueAsObject(info[0]);
    const auto outObj = Napi::Object::New(info.Env());
    float h, s, v;
    ImGui::ColorConvertRGBtoHSV(asF32(inObj.Get("x")), asF32(inObj.Get("y")), asF32(inObj.Get("z")), h, s, v);
    outObj.Set("x", fromF32(info.Env(), h));
    outObj.Set("y", fromF32(info.Env(), s));
    outObj.Set("z", fromF32(info.Env(), v));
    return outObj;
}

Napi::Value colorConvertHSVtoRGB(const Napi::CallbackInfo& info) {
    const auto inObj = valueAsObject(info[0]);
    const auto outObj = Napi::Object::New(info.Env());
    float r, g, b;
    ImGui::ColorConvertHSVtoRGB(asF32(inObj.Get("x")), asF32(inObj.Get("y")), asF32(inObj.Get("z")), r, g, b);
    outObj.Set("x", fromF32(info.Env(), r));
    outObj.Set("y", fromF32(info.Env(), g));
    outObj.Set("z", fromF32(info.Env(), b));
    return outObj;
}

Napi::Value isKeyDown(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsKeyDown(/* key */ asS32(info[0])));
}

Napi::Value isKeyPressed(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsKeyPressed(
        /* key */ asS32(info[0]),
        /* repeat */ asBoolOr(info[1], true)));
}

Napi::Value isKeyReleased(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsKeyReleased(/* key */ asS32(info[0])));
}

Napi::Value getKeyPressedAmount(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::GetKeyPressedAmount(
        /* key */ asS32(info[0]),
        /* repeat_delay */ asF32(info[1]),
        /* rate */ asF32(info[2])));
}

Napi::Value getKeyName(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), ImGui::GetKeyName(/* key */ asS32(info[0])));
}

Napi::Value setNextFrameWantCaptureKeyboard(const Napi::CallbackInfo& info) {
    ImGui::SetNextFrameWantCaptureKeyboard(/* want_capture_keyboard */ asBool(info[0]));
    return info.Env().Undefined();
}

Napi::Value isMouseDown(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsMouseDown(/* button */ asS32(info[0])));
}

Napi::Value isMouseClicked(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsMouseClicked(
        /* button */ asS32(info[0]),
        /* repeat */ asBoolOr(info[1], false)));
}

Napi::Value isMouseReleased(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsMouseReleased(/* button */ asS32(info[0])));
}

Napi::Value isMouseDoubleClicked(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsMouseDoubleClicked(/* button */ asS32(info[0])));
}

Napi::Value getMouseClickedCount(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::GetMouseClickedCount(/* button */ asS32(info[0])));
}

Napi::Value isMouseHoveringRect(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsMouseHoveringRect(
        /* r_min */ asVec2(info[0]),
        /* r_max */ asVec2(info[1]),
        /* clip */ asBoolOr(info[2], true)));
}

Napi::Value isMousePosValid(const Napi::CallbackInfo& info) {
    const auto mousePos = info[0].IsUndefined() ? std::nullopt : std::optional{ asVec2(info[0]) };
    return fromBool(info.Env(), ImGui::IsMousePosValid(
        /* mouse_pos */ mousePos.has_value() ? &mousePos.value() : nullptr));
}

Napi::Value isAnyMouseDown(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsAnyMouseDown());
}

Napi::Value getMousePos(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetMousePos());
}

Napi::Value getMousePosOnOpeningCurrentPopup(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetMousePosOnOpeningCurrentPopup());
}

Napi::Value isMouseDragging(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), ImGui::IsMouseDragging(
        /* button */ asS32(info[0]),
        /* lock_threshold */ asF32Or(info[1], -1.0f)));
}

Napi::Value getMouseDragDelta(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), ImGui::GetMouseDragDelta(
        /* button */ asS32Or(info[0], 0),
        /* lock_threshold */ asF32Or(info[1], -1.0f)));
}

Napi::Value resetMouseDragDelta(const Napi::CallbackInfo& info) {
    ImGui::ResetMouseDragDelta(/* button */ asS32Or(info[0], 0));
    return info.Env().Undefined();
}

Napi::Value getMouseCursor(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), ImGui::GetMouseCursor());
}

Napi::Value setMouseCursor(const Napi::CallbackInfo& info) {
    ImGui::SetMouseCursor(/* cursor_type */ asS32(info[0]));
    return info.Env().Undefined();
}

Napi::Value setNextFrameWantCaptureMouse(const Napi::CallbackInfo& info) {
    ImGui::SetNextFrameWantCaptureMouse(/* want_capture_mouse */ asBool(info[0]));
    return info.Env().Undefined();
}

Napi::Value getClipboardText(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), ImGui::GetClipboardText());
}

Napi::Value setClipboardText(const Napi::CallbackInfo& info) {
    ImGui::SetClipboardText(/* text */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value loadIniSettingsFromFile(const Napi::CallbackInfo& info) {
    ImGui::LoadIniSettingsFromDisk(/* ini_filename */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value loadIniSettingsFromString(const Napi::CallbackInfo& info) {
    ImGui::LoadIniSettingsFromMemory(/* ini_data */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value saveIniSettingsToFile(const Napi::CallbackInfo& info) {
    ImGui::SaveIniSettingsToDisk(/* ini_filename */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value saveIniSettingsToString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), ImGui::SaveIniSettingsToMemory());
}

Napi::Value findViewportById(const Napi::CallbackInfo& info) {
    return Viewport::create(info.Env(), ImGui::FindViewportByID(/* id */ asU32(info[0])));
}

Napi::Value findViewportByPlatformHandle(const Napi::CallbackInfo& info) {
    return Viewport::create(info.Env(), ImGui::FindViewportByPlatformHandle(/* platform_handle */ OpaquePointer::get(info[0])));
}

Napi::Value internalSetItemUsingMouseWheel(const Napi::CallbackInfo& info) {
    ImGui::SetItemUsingMouseWheel();
    return info.Env().Undefined();
}

Napi::Object initialize(Napi::Env env, Napi::Object exports) {
    DrawList::initialize(env, exports);
    Font::initialize(env, exports);
    FontAtlas::initialize(env, exports);
    FontConfig::initialize(env, exports);
    FontGlyphRangesBuilder::initialize(env, exports);
    InputTextCallbackData::initialize(env, exports);
    IO::initialize(env, exports);
    KeyData::initialize(env, exports);
    Payload::initialize(env, exports);
    SizeCallbackData::initialize(env, exports);
    Style::initialize(env, exports);
    TableColumnSortSpecs::initialize(env, exports);
    TableSortSpecs::initialize(env, exports);
    Viewport::initialize(env, exports);
    WindowClass::initialize(env, exports);
    MemoryEditor_::initialize(env, exports);

    exports.Set("getIO", Napi::Function::New(env, getIO));
    exports.Set("getStyle", Napi::Function::New(env, getStyle));
    exports.Set("showDemoWindow", Napi::Function::New(env, showDemoWindow));
    exports.Set("showMetricsWindow", Napi::Function::New(env, showMetricsWindow));
    exports.Set("showDebugLogWindow", Napi::Function::New(env, showDebugLogWindow));
    exports.Set("showStackToolWindow", Napi::Function::New(env, showStackToolWindow));
    exports.Set("showAboutWindow", Napi::Function::New(env, showAboutWindow));
    exports.Set("showStyleEditor", Napi::Function::New(env, showStyleEditor));
    exports.Set("showStyleSelector", Napi::Function::New(env, showStyleSelector));
    exports.Set("showFontSelector", Napi::Function::New(env, showFontSelector));
    exports.Set("showUserGuide", Napi::Function::New(env, showUserGuide));
    exports.Set("getVersion", Napi::Function::New(env, getVersion));
    exports.Set("styleColorsDark", Napi::Function::New(env, styleColorsDark));
    exports.Set("styleColorsLight", Napi::Function::New(env, styleColorsLight));
    exports.Set("styleColorsClassic", Napi::Function::New(env, styleColorsClassic));
    exports.Set("begin", Napi::Function::New(env, begin));
    exports.Set("end", Napi::Function::New(env, end));
    exports.Set("beginChild", Napi::Function::New(env, beginChild));
    exports.Set("endChild", Napi::Function::New(env, endChild));
    exports.Set("isWindowAppearing", Napi::Function::New(env, isWindowAppearing));
    exports.Set("isWindowCollapsed", Napi::Function::New(env, isWindowCollapsed));
    exports.Set("isWindowFocused", Napi::Function::New(env, isWindowFocused));
    exports.Set("isWindowHovered", Napi::Function::New(env, isWindowHovered));
    exports.Set("getWindowDrawList", Napi::Function::New(env, getWindowDrawList));
    exports.Set("getWindowDpiScale", Napi::Function::New(env, getWindowDpiScale));
    exports.Set("getWindowPos", Napi::Function::New(env, getWindowPos));
    exports.Set("getWindowSize", Napi::Function::New(env, getWindowSize));
    exports.Set("getWindowWidth", Napi::Function::New(env, getWindowWidth));
    exports.Set("getWindowHeight", Napi::Function::New(env, getWindowHeight));
    exports.Set("getWindowViewport", Napi::Function::New(env, getWindowViewport));
    exports.Set("setNextWindowPos", Napi::Function::New(env, setNextWindowPos));
    exports.Set("setNextWindowSize", Napi::Function::New(env, setNextWindowSize));
    exports.Set("setNextWindowSizeConstraints", Napi::Function::New(env, setNextWindowSizeConstraints));
    exports.Set("setNextWindowContentSize", Napi::Function::New(env, setNextWindowContentSize));
    exports.Set("setNextWindowCollapsed", Napi::Function::New(env, setNextWindowCollapsed));
    exports.Set("setNextWindowFocus", Napi::Function::New(env, setNextWindowFocus));
    exports.Set("setNextWindowBgAlpha", Napi::Function::New(env, setNextWindowBgAlpha));
    exports.Set("setNextWindowViewport", Napi::Function::New(env, setNextWindowViewport));
    exports.Set("setWindowPos", Napi::Function::New(env, setWindowPos));
    exports.Set("setWindowSize", Napi::Function::New(env, setWindowSize));
    exports.Set("setWindowCollapsed", Napi::Function::New(env, setWindowCollapsed));
    exports.Set("setWindowFocus", Napi::Function::New(env, setWindowFocus));
    exports.Set("setWindowFontScale", Napi::Function::New(env, setWindowFontScale));
    exports.Set("getContentRegionAvail", Napi::Function::New(env, getContentRegionAvail));
    exports.Set("getContentRegionMax", Napi::Function::New(env, getContentRegionMax));
    exports.Set("getWindowContentRegionMin", Napi::Function::New(env, getWindowContentRegionMin));
    exports.Set("getWindowContentRegionMax", Napi::Function::New(env, getWindowContentRegionMax));
    exports.Set("getScrollX", Napi::Function::New(env, getScrollX));
    exports.Set("getScrollY", Napi::Function::New(env, getScrollY));
    exports.Set("setScrollX", Napi::Function::New(env, setScrollX));
    exports.Set("setScrollY", Napi::Function::New(env, setScrollY));
    exports.Set("getScrollMaxX", Napi::Function::New(env, getScrollMaxX));
    exports.Set("getScrollMaxY", Napi::Function::New(env, getScrollMaxY));
    exports.Set("setScrollHereX", Napi::Function::New(env, setScrollHereX));
    exports.Set("setScrollHereY", Napi::Function::New(env, setScrollHereY));
    exports.Set("setScrollFromPosX", Napi::Function::New(env, setScrollFromPosX));
    exports.Set("setScrollFromPosY", Napi::Function::New(env, setScrollFromPosY));
    exports.Set("pushFont", Napi::Function::New(env, pushFont));
    exports.Set("popFont", Napi::Function::New(env, popFont));
    exports.Set("pushStyleColor", Napi::Function::New(env, pushStyleColor));
    exports.Set("popStyleColor", Napi::Function::New(env, popStyleColor));
    exports.Set("pushStyleVar", Napi::Function::New(env, pushStyleVar));
    exports.Set("popStyleVar", Napi::Function::New(env, popStyleVar));
    exports.Set("pushAllowKeyboardFocus", Napi::Function::New(env, pushAllowKeyboardFocus));
    exports.Set("popAllowKeyboardFocus", Napi::Function::New(env, popAllowKeyboardFocus));
    exports.Set("pushButtonRepeat", Napi::Function::New(env, pushButtonRepeat));
    exports.Set("popButtonRepeat", Napi::Function::New(env, popButtonRepeat));
    exports.Set("pushItemWidth", Napi::Function::New(env, pushItemWidth));
    exports.Set("popItemWidth", Napi::Function::New(env, popItemWidth));
    exports.Set("setNextItemWidth", Napi::Function::New(env, setNextItemWidth));
    exports.Set("calcItemWidth", Napi::Function::New(env, calcItemWidth));
    exports.Set("pushTextWrapPos", Napi::Function::New(env, pushTextWrapPos));
    exports.Set("popTextWrapPos", Napi::Function::New(env, popTextWrapPos));
    exports.Set("getFont", Napi::Function::New(env, getFont));
    exports.Set("getFontSize", Napi::Function::New(env, getFontSize));
    exports.Set("getFontTexUvWhitePixel", Napi::Function::New(env, getFontTexUvWhitePixel));
    exports.Set("getStyleColor", Napi::Function::New(env, getStyleColor));
    exports.Set("separator", Napi::Function::New(env, separator));
    exports.Set("sameLine", Napi::Function::New(env, sameLine));
    exports.Set("newLine", Napi::Function::New(env, newLine));
    exports.Set("spacing", Napi::Function::New(env, spacing));
    exports.Set("dummy", Napi::Function::New(env, dummy));
    exports.Set("indent", Napi::Function::New(env, indent));
    exports.Set("unindent", Napi::Function::New(env, unindent));
    exports.Set("beginGroup", Napi::Function::New(env, beginGroup));
    exports.Set("endGroup", Napi::Function::New(env, endGroup));
    exports.Set("getCursorPos", Napi::Function::New(env, getCursorPos));
    exports.Set("getCursorPosX", Napi::Function::New(env, getCursorPosX));
    exports.Set("getCursorPosY", Napi::Function::New(env, getCursorPosY));
    exports.Set("setCursorPos", Napi::Function::New(env, setCursorPos));
    exports.Set("setCursorPosX", Napi::Function::New(env, setCursorPosX));
    exports.Set("setCursorPosY", Napi::Function::New(env, setCursorPosY));
    exports.Set("getCursorStartPos", Napi::Function::New(env, getCursorStartPos));
    exports.Set("getCursorScreenPos", Napi::Function::New(env, getCursorScreenPos));
    exports.Set("setCursorScreenPos", Napi::Function::New(env, setCursorScreenPos));
    exports.Set("alignTextToFramePadding", Napi::Function::New(env, alignTextToFramePadding));
    exports.Set("getTextLineHeight", Napi::Function::New(env, getTextLineHeight));
    exports.Set("getTextLineHeightWithSpacing", Napi::Function::New(env, getTextLineHeightWithSpacing));
    exports.Set("getFrameHeight", Napi::Function::New(env, getFrameHeight));
    exports.Set("getFrameHeightWithSpacing", Napi::Function::New(env, getFrameHeightWithSpacing));
    exports.Set("pushId", Napi::Function::New(env, pushId));
    exports.Set("popId", Napi::Function::New(env, popId));
    exports.Set("getId", Napi::Function::New(env, getId));
    exports.Set("text", Napi::Function::New(env, text));
    exports.Set("textColored", Napi::Function::New(env, textColored));
    exports.Set("textDisabled", Napi::Function::New(env, textDisabled));
    exports.Set("textWrapped", Napi::Function::New(env, textWrapped));
    exports.Set("labelText", Napi::Function::New(env, labelText));
    exports.Set("bulletText", Napi::Function::New(env, bulletText));
    exports.Set("button", Napi::Function::New(env, button));
    exports.Set("smallButton", Napi::Function::New(env, smallButton));
    exports.Set("invisibleButton", Napi::Function::New(env, invisibleButton));
    exports.Set("arrowButton", Napi::Function::New(env, arrowButton));
    exports.Set("image", Napi::Function::New(env, image));
    exports.Set("imageButton", Napi::Function::New(env, imageButton));
    exports.Set("checkbox", Napi::Function::New(env, checkbox));
    exports.Set("checkboxFlags", Napi::Function::New(env, checkboxFlags));
    exports.Set("radioButton", Napi::Function::New(env, radioButton));
    exports.Set("progressBar", Napi::Function::New(env, progressBar));
    exports.Set("bullet", Napi::Function::New(env, bullet));
    exports.Set("beginCombo", Napi::Function::New(env, beginCombo));
    exports.Set("endCombo", Napi::Function::New(env, endCombo));
    exports.Set("combo", Napi::Function::New(env, combo));
    exports.Set("dragNumber", Napi::Function::New(env, dragNumber));
    exports.Set("dragNumbers", Napi::Function::New(env, dragNumbers));
    exports.Set("dragNumberRange", Napi::Function::New(env, dragNumberRange));
    exports.Set("sliderNumber", Napi::Function::New(env, sliderNumber));
    exports.Set("sliderNumbers", Napi::Function::New(env, sliderNumbers));
    exports.Set("vSliderNumber", Napi::Function::New(env, vSliderNumber));
    exports.Set("sliderAngle", Napi::Function::New(env, sliderAngle));
    exports.Set("inputText", Napi::Function::New(env, inputText));
    exports.Set("inputTextMultiline", Napi::Function::New(env, inputTextMultiline));
    exports.Set("inputTextWithHint", Napi::Function::New(env, inputTextWithHint));
    exports.Set("inputNumber", Napi::Function::New(env, inputNumber));
    exports.Set("inputNumbers", Napi::Function::New(env, inputNumbers));
    exports.Set("colorEdit3", Napi::Function::New(env, colorEdit3));
    exports.Set("colorEdit4", Napi::Function::New(env, colorEdit4));
    exports.Set("colorPicker3", Napi::Function::New(env, colorPicker3));
    exports.Set("colorPicker4", Napi::Function::New(env, colorPicker4));
    exports.Set("colorButton", Napi::Function::New(env, colorButton));
    exports.Set("setColorEditOptions", Napi::Function::New(env, setColorEditOptions));
    exports.Set("treeNode", Napi::Function::New(env, treeNode));
    exports.Set("treeNodeEx", Napi::Function::New(env, treeNodeEx));
    exports.Set("treePush", Napi::Function::New(env, treePush));
    exports.Set("treePop", Napi::Function::New(env, treePop));
    exports.Set("getTreeNodeToLabelSpacing", Napi::Function::New(env, getTreeNodeToLabelSpacing));
    exports.Set("collapsingHeader", Napi::Function::New(env, collapsingHeader));
    exports.Set("setNextItemOpen", Napi::Function::New(env, setNextItemOpen));
    exports.Set("selectable", Napi::Function::New(env, selectable));
    exports.Set("beginListBox", Napi::Function::New(env, beginListBox));
    exports.Set("endListBox", Napi::Function::New(env, endListBox));
    exports.Set("listBox", Napi::Function::New(env, listBox));
    exports.Set("plotLines", Napi::Function::New(env, plotLines));
    exports.Set("plotHistogram", Napi::Function::New(env, plotHistogram));
    exports.Set("value", Napi::Function::New(env, value));
    exports.Set("beginMenuBar", Napi::Function::New(env, beginMenuBar));
    exports.Set("endMenuBar", Napi::Function::New(env, endMenuBar));
    exports.Set("beginMainMenuBar", Napi::Function::New(env, beginMainMenuBar));
    exports.Set("endMainMenuBar", Napi::Function::New(env, endMainMenuBar));
    exports.Set("beginMenu", Napi::Function::New(env, beginMenu));
    exports.Set("endMenu", Napi::Function::New(env, endMenu));
    exports.Set("menuItem", Napi::Function::New(env, menuItem));
    exports.Set("beginTooltip", Napi::Function::New(env, beginTooltip));
    exports.Set("endTooltip", Napi::Function::New(env, endTooltip));
    exports.Set("setTooltip", Napi::Function::New(env, setTooltip));
    exports.Set("beginPopup", Napi::Function::New(env, beginPopup));
    exports.Set("beginPopupModal", Napi::Function::New(env, beginPopupModal));
    exports.Set("endPopup", Napi::Function::New(env, endPopup));
    exports.Set("openPopup", Napi::Function::New(env, openPopup));
    exports.Set("openPopupOnItemClick", Napi::Function::New(env, openPopupOnItemClick));
    exports.Set("closeCurrentPopup", Napi::Function::New(env, closeCurrentPopup));
    exports.Set("beginPopupContextItem", Napi::Function::New(env, beginPopupContextItem));
    exports.Set("beginPopupContextWindow", Napi::Function::New(env, beginPopupContextWindow));
    exports.Set("beginPopupContextVoid", Napi::Function::New(env, beginPopupContextVoid));
    exports.Set("isPopupOpen", Napi::Function::New(env, isPopupOpen));
    exports.Set("beginTable", Napi::Function::New(env, beginTable));
    exports.Set("endTable", Napi::Function::New(env, endTable));
    exports.Set("tableNextRow", Napi::Function::New(env, tableNextRow));
    exports.Set("tableNextColumn", Napi::Function::New(env, tableNextColumn));
    exports.Set("tableSetColumnIndex", Napi::Function::New(env, tableSetColumnIndex));
    exports.Set("tableSetupColumn", Napi::Function::New(env, tableSetupColumn));
    exports.Set("tableSetupScrollFreeze", Napi::Function::New(env, tableSetupScrollFreeze));
    exports.Set("tableHeadersRow", Napi::Function::New(env, tableHeadersRow));
    exports.Set("tableHeader", Napi::Function::New(env, tableHeader));
    exports.Set("tableGetSortSpecs", Napi::Function::New(env, tableGetSortSpecs));
    exports.Set("tableGetColumnCount", Napi::Function::New(env, tableGetColumnCount));
    exports.Set("tableGetColumnIndex", Napi::Function::New(env, tableGetColumnIndex));
    exports.Set("tableGetRowIndex", Napi::Function::New(env, tableGetRowIndex));
    exports.Set("tableGetColumnName", Napi::Function::New(env, tableGetColumnName));
    exports.Set("tableGetColumnFlags", Napi::Function::New(env, tableGetColumnFlags));
    exports.Set("tableSetColumnEnabled", Napi::Function::New(env, tableSetColumnEnabled));
    exports.Set("tableSetBgColor", Napi::Function::New(env, tableSetBgColor));
    exports.Set("columns", Napi::Function::New(env, columns));
    exports.Set("nextColumn", Napi::Function::New(env, nextColumn));
    exports.Set("getColumnIndex", Napi::Function::New(env, getColumnIndex));
    exports.Set("getColumnWidth", Napi::Function::New(env, getColumnWidth));
    exports.Set("setColumnWidth", Napi::Function::New(env, setColumnWidth));
    exports.Set("getColumnOffset", Napi::Function::New(env, getColumnOffset));
    exports.Set("setColumnOffset", Napi::Function::New(env, setColumnOffset));
    exports.Set("getColumnsCount", Napi::Function::New(env, getColumnsCount));
    exports.Set("beginTabBar", Napi::Function::New(env, beginTabBar));
    exports.Set("endTabBar", Napi::Function::New(env, endTabBar));
    exports.Set("beginTabItem", Napi::Function::New(env, beginTabItem));
    exports.Set("endTabItem", Napi::Function::New(env, endTabItem));
    exports.Set("tabItemButton", Napi::Function::New(env, tabItemButton));
    exports.Set("setTabItemClosed", Napi::Function::New(env, setTabItemClosed));
    exports.Set("dockSpace", Napi::Function::New(env, dockSpace));
    exports.Set("dockSpaceOverViewport", Napi::Function::New(env, dockSpaceOverViewport));
    exports.Set("setNextWindowDockId", Napi::Function::New(env, setNextWindowDockId));
    exports.Set("setNextWindowClass", Napi::Function::New(env, setNextWindowClass));
    exports.Set("getWindowDockId", Napi::Function::New(env, getWindowDockId));
    exports.Set("isWindowDocked", Napi::Function::New(env, isWindowDocked));
    exports.Set("logToTTY", Napi::Function::New(env, logToTTY));
    exports.Set("logToFile", Napi::Function::New(env, logToFile));
    exports.Set("logToClipboard", Napi::Function::New(env, logToClipboard));
    exports.Set("logFinish", Napi::Function::New(env, logFinish));
    exports.Set("logButtons", Napi::Function::New(env, logButtons));
    exports.Set("logText", Napi::Function::New(env, logText));
    exports.Set("beginDragDropSource", Napi::Function::New(env, beginDragDropSource));
    exports.Set("setDragDropPayload", Napi::Function::New(env, setDragDropPayload));
    exports.Set("endDragDropSource", Napi::Function::New(env, endDragDropSource));
    exports.Set("beginDragDropTarget", Napi::Function::New(env, beginDragDropTarget));
    exports.Set("acceptDragDropPayload", Napi::Function::New(env, acceptDragDropPayload));
    exports.Set("endDragDropTarget", Napi::Function::New(env, endDragDropTarget));
    exports.Set("getDragDropPayload", Napi::Function::New(env, getDragDropPayload));
    exports.Set("beginDisabled", Napi::Function::New(env, beginDisabled));
    exports.Set("endDisabled", Napi::Function::New(env, endDisabled));
    exports.Set("pushClipRect", Napi::Function::New(env, pushClipRect));
    exports.Set("popClipRect", Napi::Function::New(env, popClipRect));
    exports.Set("setItemDefaultFocus", Napi::Function::New(env, setItemDefaultFocus));
    exports.Set("setKeyboardFocusHere", Napi::Function::New(env, setKeyboardFocusHere));
    exports.Set("isItemHovered", Napi::Function::New(env, isItemHovered));
    exports.Set("isItemActive", Napi::Function::New(env, isItemActive));
    exports.Set("isItemFocused", Napi::Function::New(env, isItemFocused));
    exports.Set("isItemClicked", Napi::Function::New(env, isItemClicked));
    exports.Set("isItemVisible", Napi::Function::New(env, isItemVisible));
    exports.Set("isItemEdited", Napi::Function::New(env, isItemEdited));
    exports.Set("isItemActivated", Napi::Function::New(env, isItemActivated));
    exports.Set("isItemDeactivated", Napi::Function::New(env, isItemDeactivated));
    exports.Set("isItemDeactivatedAfterEdit", Napi::Function::New(env, isItemDeactivatedAfterEdit));
    exports.Set("isItemToggledOpen", Napi::Function::New(env, isItemToggledOpen));
    exports.Set("isAnyItemHovered", Napi::Function::New(env, isAnyItemHovered));
    exports.Set("isAnyItemActive", Napi::Function::New(env, isAnyItemActive));
    exports.Set("isAnyItemFocused", Napi::Function::New(env, isAnyItemFocused));
    exports.Set("getItemRectMin", Napi::Function::New(env, getItemRectMin));
    exports.Set("getItemRectMax", Napi::Function::New(env, getItemRectMax));
    exports.Set("getItemRectSize", Napi::Function::New(env, getItemRectSize));
    exports.Set("setItemAllowOverlap", Napi::Function::New(env, setItemAllowOverlap));
    exports.Set("getMainViewport", Napi::Function::New(env, getMainViewport));
    exports.Set("getBackgroundDrawList", Napi::Function::New(env, getBackgroundDrawList));
    exports.Set("getForegroundDrawList", Napi::Function::New(env, getForegroundDrawList));
    exports.Set("isRectVisible", Napi::Function::New(env, isRectVisible));
    exports.Set("getTime", Napi::Function::New(env, getTime));
    exports.Set("getFrameCount", Napi::Function::New(env, getFrameCount));
    exports.Set("getStyleColorName", Napi::Function::New(env, getStyleColorName));
    exports.Set("beginChildFrame", Napi::Function::New(env, beginChildFrame));
    exports.Set("endChildFrame", Napi::Function::New(env, endChildFrame));
    exports.Set("calcTextSize", Napi::Function::New(env, calcTextSize));
    exports.Set("colorConvertRGBtoHSV", Napi::Function::New(env, colorConvertRGBtoHSV));
    exports.Set("colorConvertHSVtoRGB", Napi::Function::New(env, colorConvertHSVtoRGB));
    exports.Set("isKeyDown", Napi::Function::New(env, isKeyDown));
    exports.Set("isKeyPressed", Napi::Function::New(env, isKeyPressed));
    exports.Set("isKeyReleased", Napi::Function::New(env, isKeyReleased));
    exports.Set("getKeyPressedAmount", Napi::Function::New(env, getKeyPressedAmount));
    exports.Set("getKeyName", Napi::Function::New(env, getKeyName));
    exports.Set("setNextFrameWantCaptureKeyboard", Napi::Function::New(env, setNextFrameWantCaptureKeyboard));
    exports.Set("isMouseDown", Napi::Function::New(env, isMouseDown));
    exports.Set("isMouseClicked", Napi::Function::New(env, isMouseClicked));
    exports.Set("isMouseReleased", Napi::Function::New(env, isMouseReleased));
    exports.Set("isMouseDoubleClicked", Napi::Function::New(env, isMouseDoubleClicked));
    exports.Set("getMouseClickedCount", Napi::Function::New(env, getMouseClickedCount));
    exports.Set("isMouseHoveringRect", Napi::Function::New(env, isMouseHoveringRect));
    exports.Set("isMousePosValid", Napi::Function::New(env, isMousePosValid));
    exports.Set("isAnyMouseDown", Napi::Function::New(env, isAnyMouseDown));
    exports.Set("getMousePos", Napi::Function::New(env, getMousePos));
    exports.Set("getMousePosOnOpeningCurrentPopup", Napi::Function::New(env, getMousePosOnOpeningCurrentPopup));
    exports.Set("isMouseDragging", Napi::Function::New(env, isMouseDragging));
    exports.Set("getMouseDragDelta", Napi::Function::New(env, getMouseDragDelta));
    exports.Set("resetMouseDragDelta", Napi::Function::New(env, resetMouseDragDelta));
    exports.Set("getMouseCursor", Napi::Function::New(env, getMouseCursor));
    exports.Set("setMouseCursor", Napi::Function::New(env, setMouseCursor));
    exports.Set("setNextFrameWantCaptureMouse", Napi::Function::New(env, setNextFrameWantCaptureMouse));
    exports.Set("getClipboardText", Napi::Function::New(env, getClipboardText));
    exports.Set("setClipboardText", Napi::Function::New(env, setClipboardText));
    exports.Set("loadIniSettingsFromFile", Napi::Function::New(env, loadIniSettingsFromFile));
    exports.Set("loadIniSettingsFromString", Napi::Function::New(env, loadIniSettingsFromString));
    exports.Set("saveIniSettingsToFile", Napi::Function::New(env, saveIniSettingsToFile));
    exports.Set("saveIniSettingsToString", Napi::Function::New(env, saveIniSettingsToString));
    exports.Set("findViewportById", Napi::Function::New(env, findViewportById));
    exports.Set("findViewportByPlatformHandle", Napi::Function::New(env, findViewportByPlatformHandle));
    exports.Set("internalSetItemUsingMouseWheel", Napi::Function::New(env, internalSetItemUsingMouseWheel));

    return exports;
}

}
