#include "core/type/instanceOf.hpp"
#include "core/opaquePointer.hpp"
#include "imgui/payload.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core;
using namespace core::type::convert;

namespace imgui {

Napi::FunctionReference Payload::m_ctor;

Napi::Object Payload::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "Payload";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&Payload::getTypeId>("typeId"),
        InstanceAccessor<&Payload::toDebugString>("debugString"),
        InstanceMethod<&Payload::ref>("ref"),
        InstanceMethod<&Payload::unref>("unref"),

        InstanceMethod<&Payload::equals>("equals"),
        InstanceAccessor<&Payload::getData>("data"),
        InstanceMethod<&Payload::clear>("clear"),
        InstanceMethod<&Payload::isDataType>("isDataType"),
        InstanceMethod<&Payload::isPreview>("isPreview"),
        InstanceMethod<&Payload::isDelivery>("isDelivery")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object Payload::create(Napi::Env, ImGuiPayload* val) {
    const auto obj = m_ctor.New({ m_ctor.Value() });
    unwrap(obj)->m_val = val;
    return obj;
}

Payload::Payload(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Payload>{ info } {}

Napi::Value Payload::getTypeId(const Napi::CallbackInfo& info) {
    return core::type::fromTypeId<Payload>(info.Env());
}

Napi::Value Payload::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value Payload::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value Payload::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value Payload::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == Payload::get(info[0]));
}

Napi::Value Payload::getData(const Napi::CallbackInfo& info) {
    return Napi::ArrayBuffer::New(info.Env(), m_val->Data, m_val->DataSize);
}

Napi::Value Payload::clear(const Napi::CallbackInfo& info) {
    m_val->Clear();
    return info.Env().Undefined();
}

Napi::Value Payload::isDataType(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->IsDataType(/* type */ asStrUtf8(info[0]).c_str()));
}

Napi::Value Payload::isPreview(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->IsPreview());
}

Napi::Value Payload::isDelivery(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->IsDelivery());
}

}
