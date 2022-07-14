#pragma once

#include <napi.h>

struct ImGuiViewport;

namespace imgui {

class Viewport final : public Napi::ObjectWrap<Viewport> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env env, ImGuiViewport* val);

    static Viewport* unwrap(const Napi::Object& obj) {
        return Viewport::Unwrap(obj);
    }

    static Viewport* unwrap(const Napi::Value& val) {
        return Viewport::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiViewport* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiViewport* get() {
        return m_val;
    }

    Viewport(const Napi::CallbackInfo& info);
    virtual ~Viewport() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImGuiViewport* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getId(const Napi::CallbackInfo& info);
    void setId(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFlags(const Napi::CallbackInfo& info);
    void setFlags(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getPos(const Napi::CallbackInfo& info);
    void setPos(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getSize(const Napi::CallbackInfo& info);
    void setSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWorkPos(const Napi::CallbackInfo& info);
    void setWorkPos(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWorkSize(const Napi::CallbackInfo& info);
    void setWorkSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDpiScale(const Napi::CallbackInfo& info);
    void setDpiScale(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getParentViewportId(const Napi::CallbackInfo& info);
    void setParentViewportId(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getPlatformHandle(const Napi::CallbackInfo& info);
    Napi::Value getPlatformHandleRaw(const Napi::CallbackInfo& info);
    Napi::Value getCenter(const Napi::CallbackInfo& info);
    Napi::Value getWorkCenter(const Napi::CallbackInfo& info);
};

}
