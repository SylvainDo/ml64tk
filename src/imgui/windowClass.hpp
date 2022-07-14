#pragma once

#include <napi.h>

struct ImGuiWindowClass;

namespace imgui {

class WindowClass final : public Napi::ObjectWrap<WindowClass> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env, ImGuiWindowClass* val);

    static WindowClass* unwrap(const Napi::Object& obj) {
        return WindowClass::Unwrap(obj);
    }

    static WindowClass* unwrap(const Napi::Value& val) {
        return WindowClass::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiWindowClass* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiWindowClass* get() {
        return m_val;
    }

    WindowClass(const Napi::CallbackInfo& info);
    virtual ~WindowClass();

private:
    static Napi::FunctionReference m_ctor;
    ImGuiWindowClass* m_val;
    bool m_alloc{};

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getClassId(const Napi::CallbackInfo& info);
    void setClassId(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getParentViewportId(const Napi::CallbackInfo& info);
    void setParentViewportId(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getViewportFlagsOverrideSet(const Napi::CallbackInfo& info);
    void setViewportFlagsOverrideSet(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getViewportFlagsOverrideClear(const Napi::CallbackInfo& info);
    void setViewportFlagsOverrideClear(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getTabItemFlagsOverrideSet(const Napi::CallbackInfo& info);
    void setTabItemFlagsOverrideSet(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDockNodeFlagsOverrideSet(const Napi::CallbackInfo& info);
    void setDockNodeFlagsOverrideSet(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDockingAlwaysTabBar(const Napi::CallbackInfo& info);
    void setDockingAlwaysTabBar(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDockingAllowUnclassed(const Napi::CallbackInfo& info);
    void setDockingAllowUnclassed(const Napi::CallbackInfo&, const Napi::Value& val);
};

}
