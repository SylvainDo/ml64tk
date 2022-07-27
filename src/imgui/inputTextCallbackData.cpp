#include "core/instanceOf.hpp"
#include "imgui/inputTextCallbackData.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core;

namespace imgui {

Napi::FunctionReference InputTextCallbackData::m_ctor;

Napi::Object InputTextCallbackData::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "InputTextCallbackData";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&InputTextCallbackData::getTypeId>("typeId"),
        InstanceAccessor<&InputTextCallbackData::toDebugString>("debugString"),
        InstanceMethod<&InputTextCallbackData::ref>("ref"),
        InstanceMethod<&InputTextCallbackData::unref>("unref"),

        InstanceMethod<&InputTextCallbackData::equals>("equals"),
        InstanceAccessor<&InputTextCallbackData::getEventFlag>("eventFlag"),
        InstanceAccessor<&InputTextCallbackData::getFlags>("flags"),
        InstanceAccessor<&InputTextCallbackData::getEventChar, &InputTextCallbackData::setEventChar>("eventChar"),
        InstanceAccessor<&InputTextCallbackData::getEventKey>("eventKey"),
        InstanceAccessor<&InputTextCallbackData::getBuf, &InputTextCallbackData::setBuf>("buf"),
        InstanceAccessor<&InputTextCallbackData::getCursorPos, &InputTextCallbackData::setCursorPos>("cursorPos"),
        InstanceAccessor<&InputTextCallbackData::getSelectionStart, &InputTextCallbackData::setSelectionStart>("selectionStart"),
        InstanceAccessor<&InputTextCallbackData::getSelectionEnd, &InputTextCallbackData::setSelectionEnd>("selectionEnd"),
        InstanceMethod<&InputTextCallbackData::deleteChars>("deleteChars"),
        InstanceMethod<&InputTextCallbackData::insertChars>("insertChars"),
        InstanceMethod<&InputTextCallbackData::selectAll>("selectAll"),
        InstanceMethod<&InputTextCallbackData::clearSelection>("clearSelection"),
        InstanceMethod<&InputTextCallbackData::hasSelection>("hasSelection")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object InputTextCallbackData::create(Napi::Env, ImGuiInputTextCallbackData* val) {
    const auto obj = m_ctor.New({});
    unwrap(obj)->m_val = val;
    return obj;
}

InputTextCallbackData::InputTextCallbackData(const Napi::CallbackInfo& info) : Napi::ObjectWrap<InputTextCallbackData>{ info } {}

Napi::Value InputTextCallbackData::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<InputTextCallbackData>(info.Env());
}

Napi::Value InputTextCallbackData::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("ImGui.InputTextCallbackData (this={}; instance={})",
        fmt::ptr(this), fmt::ptr(m_val)));
}

Napi::Value InputTextCallbackData::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value InputTextCallbackData::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value InputTextCallbackData::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == InputTextCallbackData::get(info[0]));
}

Napi::Value InputTextCallbackData::getEventFlag(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->EventFlag);
}

Napi::Value InputTextCallbackData::getFlags(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->Flags);
}

Napi::Value InputTextCallbackData::getEventChar(const Napi::CallbackInfo& info) {
    return fromU16(info.Env(), m_val->EventChar);
}

void InputTextCallbackData::setEventChar(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->EventChar = asU16(val);
}

Napi::Value InputTextCallbackData::getEventKey(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->EventKey);
}

Napi::Value InputTextCallbackData::getBuf(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), m_val->Buf);
}

void InputTextCallbackData::setBuf(const Napi::CallbackInfo&, const Napi::Value& val) {
    const auto str = asStrUtf8(val);
    m_val->BufTextLen = std::min(m_val->BufSize, static_cast<int>(str.size()));
    std::strncpy(m_val->Buf, str.data(), m_val->BufTextLen);
    m_val->BufDirty = true;
}

Napi::Value InputTextCallbackData::getCursorPos(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->CursorPos);
}

void InputTextCallbackData::setCursorPos(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->CursorPos = asS32(val);
}

Napi::Value InputTextCallbackData::getSelectionStart(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->SelectionStart);
}

void InputTextCallbackData::setSelectionStart(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->SelectionStart = asS32(val);
}

Napi::Value InputTextCallbackData::getSelectionEnd(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->SelectionEnd);
}

void InputTextCallbackData::setSelectionEnd(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->SelectionEnd = asS32(val);
}

Napi::Value InputTextCallbackData::deleteChars(const Napi::CallbackInfo& info) {
    m_val->DeleteChars(
        /* pos */ asS32(info[0]),
        /* bytes_count */ asS32(info[1]));
    return info.Env().Undefined();
}

Napi::Value InputTextCallbackData::insertChars(const Napi::CallbackInfo& info) {
    m_val->InsertChars(
        /* pos */ asS32(info[0]),
        /* text */ asStrUtf8(info[1]).c_str());
    return info.Env().Undefined();
}

Napi::Value InputTextCallbackData::selectAll(const Napi::CallbackInfo& info) {
    m_val->SelectAll();
    return info.Env().Undefined();
}

Napi::Value InputTextCallbackData::clearSelection(const Napi::CallbackInfo& info) {
    m_val->ClearSelection();
    return info.Env().Undefined();
}

Napi::Value InputTextCallbackData::hasSelection(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->HasSelection());
}

}
