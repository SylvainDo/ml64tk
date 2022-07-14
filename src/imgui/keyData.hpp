#pragma once

#include <napi.h>

struct ImGuiKeyData;

namespace imgui {

class KeyData final : public Napi::ObjectWrap<KeyData> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env env, ImGuiKeyData* val);

    static KeyData* unwrap(const Napi::Object& obj) {
        return KeyData::Unwrap(obj);
    }

    static KeyData* unwrap(const Napi::Value& val) {
        return KeyData::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiKeyData* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiKeyData* get() {
        return m_val;
    }

    KeyData(const Napi::CallbackInfo& info);
    virtual ~KeyData() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImGuiKeyData* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getDown(const Napi::CallbackInfo& info);
    void setDown(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDownDuration(const Napi::CallbackInfo& info);
    void setDownDuration(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDownDurationPrev(const Napi::CallbackInfo& info);
    void setDownDurationPrev(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getAnalogValue(const Napi::CallbackInfo& info);
    void setAnalogValue(const Napi::CallbackInfo&, const Napi::Value& val);
};

}
