#pragma once

#include <napi.h>

struct ImGuiPayload;

namespace imgui {

class Payload final : public Napi::ObjectWrap<Payload> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env, ImGuiPayload* val);

    static Payload* unwrap(const Napi::Object& obj) {
        return Payload::Unwrap(obj);
    }

    static Payload* unwrap(const Napi::Value& val) {
        return Payload::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiPayload* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiPayload* get() {
        return m_val;
    }

    Payload(const Napi::CallbackInfo& info);
    virtual ~Payload() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImGuiPayload* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getData(const Napi::CallbackInfo& info);
    Napi::Value clear(const Napi::CallbackInfo& info);
    Napi::Value isDataType(const Napi::CallbackInfo& info);
    Napi::Value isPreview(const Napi::CallbackInfo& info);
    Napi::Value isDelivery(const Napi::CallbackInfo& info);
};

}
