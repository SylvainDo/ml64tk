#pragma once

#include <napi.h>

struct ImGuiInputTextCallbackData;

namespace imgui {

class InputTextCallbackData final : public Napi::ObjectWrap<InputTextCallbackData> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env, ImGuiInputTextCallbackData* val);

    static InputTextCallbackData* unwrap(const Napi::Object& obj) {
        return InputTextCallbackData::Unwrap(obj);
    }

    static InputTextCallbackData* unwrap(const Napi::Value& val) {
        return InputTextCallbackData::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiInputTextCallbackData* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiInputTextCallbackData* get() {
        return m_val;
    }

    InputTextCallbackData(const Napi::CallbackInfo& info);
    virtual ~InputTextCallbackData() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImGuiInputTextCallbackData* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getEventFlag(const Napi::CallbackInfo& info);
    Napi::Value getFlags(const Napi::CallbackInfo& info);
    Napi::Value getEventChar(const Napi::CallbackInfo& info);
    void setEventChar(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getEventKey(const Napi::CallbackInfo& info);
    Napi::Value getBuf(const Napi::CallbackInfo& info);
    void setBuf(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getCursorPos(const Napi::CallbackInfo& info);
    void setCursorPos(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getSelectionStart(const Napi::CallbackInfo& info);
    void setSelectionStart(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getSelectionEnd(const Napi::CallbackInfo& info);
    void setSelectionEnd(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value deleteChars(const Napi::CallbackInfo& info);
    Napi::Value insertChars(const Napi::CallbackInfo& info);
    Napi::Value selectAll(const Napi::CallbackInfo& info);
    Napi::Value clearSelection(const Napi::CallbackInfo& info);
    Napi::Value hasSelection(const Napi::CallbackInfo& info);
};

}
