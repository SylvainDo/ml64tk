#pragma once

#include <napi.h>

struct ImGuiSizeCallbackData;

namespace imgui {

class SizeCallbackData final : public Napi::ObjectWrap<SizeCallbackData> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env env, ImGuiSizeCallbackData* val);

    static SizeCallbackData* unwrap(const Napi::Object& obj) {
        return SizeCallbackData::Unwrap(obj);
    }

    static SizeCallbackData* unwrap(const Napi::Value& val) {
        return SizeCallbackData::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiSizeCallbackData* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiSizeCallbackData* get() {
        return m_val;
    }

    SizeCallbackData(const Napi::CallbackInfo& info);
    virtual ~SizeCallbackData() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImGuiSizeCallbackData* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getPos(const Napi::CallbackInfo& info);
    Napi::Value getCurrentSize(const Napi::CallbackInfo& info);
    Napi::Value getDesiredSize(const Napi::CallbackInfo& info);
    void setDesiredSize(const Napi::CallbackInfo&, const Napi::Value& val);
};

}
