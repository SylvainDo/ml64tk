#include "core/instanceOf.hpp"
#include "core/opaquePointer.hpp"
#include "imgui/convert.hpp"
#include "imgui/viewport.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core;

namespace imgui {

Napi::FunctionReference Viewport::m_ctor;

Napi::Object Viewport::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "Viewport";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&Viewport::getTypeId>("typeId"),
        InstanceAccessor<&Viewport::toDebugString>("debugString"),
        InstanceMethod<&Viewport::ref>("ref"),
        InstanceMethod<&Viewport::unref>("unref"),

        InstanceMethod<&Viewport::equals>("equals"),
        InstanceAccessor<&Viewport::getId, &Viewport::setId>("id"),
        InstanceAccessor<&Viewport::getFlags, &Viewport::setFlags>("flags"),
        InstanceAccessor<&Viewport::getPos, &Viewport::setPos>("pos"),
        InstanceAccessor<&Viewport::getSize, &Viewport::setSize>("size"),
        InstanceAccessor<&Viewport::getWorkPos, &Viewport::setWorkPos>("workPos"),
        InstanceAccessor<&Viewport::getWorkSize, &Viewport::setWorkSize>("workSize"),
        InstanceAccessor<&Viewport::getDpiScale, &Viewport::setDpiScale>("dpiScale"),
        InstanceAccessor<&Viewport::getParentViewportId, &Viewport::setParentViewportId>("parentViewportId"),
        InstanceAccessor<&Viewport::getPlatformHandle>("platformHandle"),
        InstanceAccessor<&Viewport::getPlatformHandleRaw>("platformHandleRaw"),
        InstanceMethod<&Viewport::getCenter>("getCenter"),
        InstanceMethod<&Viewport::getWorkCenter>("getWorkCenter")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object Viewport::create(Napi::Env, ImGuiViewport* val) {
    const auto obj = m_ctor.New({});
    unwrap(obj)->m_val = val;
    return obj;
}

Viewport::Viewport(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Viewport>{ info } {}

Napi::Value Viewport::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<Viewport>(info.Env());
}

Napi::Value Viewport::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value Viewport::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value Viewport::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value Viewport::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == Viewport::get(info[0]));
}

Napi::Value Viewport::getId(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_val->ID);
}

void Viewport::setId(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ID = asU32(val);
}

Napi::Value Viewport::getFlags(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->Flags);
}

void Viewport::setFlags(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->Flags = asS32(val);
}

Napi::Value Viewport::getPos(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->Pos);
}

void Viewport::setPos(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->Pos = asVec2(val);
}

Napi::Value Viewport::getSize(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->Size);
}

void Viewport::setSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->Size = asVec2(val);
}

Napi::Value Viewport::getWorkPos(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->WorkPos);
}

void Viewport::setWorkPos(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WorkPos = asVec2(val);
}

Napi::Value Viewport::getWorkSize(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->WorkSize);
}

void Viewport::setWorkSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WorkSize = asVec2(val);
}

Napi::Value Viewport::getDpiScale(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->DpiScale);
}

void Viewport::setDpiScale(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DpiScale = asF32(val);
}

Napi::Value Viewport::getParentViewportId(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_val->ParentViewportId);
}

void Viewport::setParentViewportId(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ParentViewportId = asU32(val);
}

Napi::Value Viewport::getPlatformHandle(const Napi::CallbackInfo& info) {
    return OpaquePointer::create(info.Env(), m_val->PlatformHandle);
}

Napi::Value Viewport::getPlatformHandleRaw(const Napi::CallbackInfo& info) {
    return OpaquePointer::create(info.Env(), m_val->PlatformHandleRaw);
}

Napi::Value Viewport::getCenter(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->GetCenter());
}

Napi::Value Viewport::getWorkCenter(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->GetWorkCenter());
}

}
