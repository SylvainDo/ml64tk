#include "core/instanceOf.hpp"
#include "imgui/windowClass.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core;

namespace imgui {

Napi::FunctionReference WindowClass::m_ctor;

Napi::Object WindowClass::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "WindowClass";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&WindowClass::getTypeId>("typeId"),
        InstanceAccessor<&WindowClass::toDebugString>("debugString"),
        InstanceMethod<&WindowClass::ref>("ref"),
        InstanceMethod<&WindowClass::unref>("unref"),

        InstanceMethod<&WindowClass::equals>("equals"),
        InstanceAccessor<&WindowClass::getClassId, &WindowClass::setClassId>("classId"),
        InstanceAccessor<&WindowClass::getParentViewportId, &WindowClass::setParentViewportId>("parentViewportId"),
        InstanceAccessor<&WindowClass::getViewportFlagsOverrideSet, &WindowClass::setViewportFlagsOverrideSet>("viewportFlagsOverrideSet"),
        InstanceAccessor<&WindowClass::getViewportFlagsOverrideClear, &WindowClass::setViewportFlagsOverrideClear>("viewportFlagsOverrideClear"),
        InstanceAccessor<&WindowClass::getTabItemFlagsOverrideSet, &WindowClass::setTabItemFlagsOverrideSet>("tabItemFlagsOverrideSet"),
        InstanceAccessor<&WindowClass::getDockNodeFlagsOverrideSet, &WindowClass::setDockNodeFlagsOverrideSet>("dockNodeFlagsOverrideSet"),
        InstanceAccessor<&WindowClass::getDockingAlwaysTabBar, &WindowClass::setDockingAlwaysTabBar>("dockingAlwaysTabBar"),
        InstanceAccessor<&WindowClass::getDockingAllowUnclassed, &WindowClass::setDockingAllowUnclassed>("dockingAllowUnclassed")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object WindowClass::create(Napi::Env, ImGuiWindowClass* val) {
    const auto obj = m_ctor.New({ m_ctor.Value() });
    unwrap(obj)->m_val = val;
    return obj;
}

WindowClass::WindowClass(const Napi::CallbackInfo& info) : Napi::ObjectWrap<WindowClass>{ info } {
    if (info[0] != m_ctor.Value()) {
        m_val = new ImGuiWindowClass{};
        m_alloc = true;
    }
}

WindowClass::~WindowClass() {
    if (m_alloc) delete m_val;
}

Napi::Value WindowClass::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<WindowClass>(info.Env());
}

Napi::Value WindowClass::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value WindowClass::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value WindowClass::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value WindowClass::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == WindowClass::get(info[0]));
}

Napi::Value WindowClass::getClassId(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_val->ClassId);
}

void WindowClass::setClassId(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ClassId = asU32(val);
}

Napi::Value WindowClass::getParentViewportId(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_val->ParentViewportId);
}

void WindowClass::setParentViewportId(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ParentViewportId = asU32(val);
}

Napi::Value WindowClass::getViewportFlagsOverrideSet(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->ViewportFlagsOverrideSet);
}

void WindowClass::setViewportFlagsOverrideSet(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ViewportFlagsOverrideSet = asS32(val);
}

Napi::Value WindowClass::getViewportFlagsOverrideClear(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->ViewportFlagsOverrideClear);
}

void WindowClass::setViewportFlagsOverrideClear(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ViewportFlagsOverrideClear = asS32(val);
}

Napi::Value WindowClass::getTabItemFlagsOverrideSet(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->TabItemFlagsOverrideSet);
}

void WindowClass::setTabItemFlagsOverrideSet(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->TabItemFlagsOverrideSet = asS32(val);
}

Napi::Value WindowClass::getDockNodeFlagsOverrideSet(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->DockNodeFlagsOverrideSet);
}

void WindowClass::setDockNodeFlagsOverrideSet(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DockNodeFlagsOverrideSet = asS32(val);
}

Napi::Value WindowClass::getDockingAlwaysTabBar(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->DockingAlwaysTabBar);
}

void WindowClass::setDockingAlwaysTabBar(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DockingAlwaysTabBar = asBool(val);
}

Napi::Value WindowClass::getDockingAllowUnclassed(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->DockingAllowUnclassed);
}

void WindowClass::setDockingAllowUnclassed(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DockingAllowUnclassed = asBool(val);
}

}
