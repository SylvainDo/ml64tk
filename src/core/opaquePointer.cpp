#include "core/instanceOf.hpp"
#include "core/opaquePointer.hpp"

#include <fmt/format.h>

namespace core {

Napi::FunctionReference OpaquePointer::m_ctor;

Napi::Object OpaquePointer::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "OpaquePointer";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&OpaquePointer::getTypeId>("typeId"),
        InstanceAccessor<&OpaquePointer::toDebugString>("debugString"),
        InstanceMethod<&OpaquePointer::ref>("ref"),
        InstanceMethod<&OpaquePointer::unref>("unref"),

        InstanceMethod<&OpaquePointer::equals>("equals"),
        StaticMethod<&OpaquePointer::fromPointer>("fromPointer"),
        StaticMethod<&OpaquePointer::fromObject>("fromObject"),
        InstanceAccessor<&OpaquePointer::getRawPointer>("rawPointer"),
        InstanceMethod<&OpaquePointer::asPointer>("asPointer"),
        InstanceMethod<&OpaquePointer::asObject>("asObject"),
        InstanceAccessor<&OpaquePointer::release>("release")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object OpaquePointer::create(Napi::Env, void* val, Type type) {
    const auto obj = m_ctor.New({});
    auto instance = unwrap(obj);
    instance->m_val = val;
    instance->m_type = type;
    return obj;
}

Napi::Object OpaquePointer::create(Napi::Env, const Napi::Object& val) {
    const auto obj = m_ctor.New({});
    auto instance = unwrap(obj);
    auto ref = new Napi::ObjectReference;
    *ref = Napi::Persistent(val);
    instance->m_val = ref;
    instance->m_type = Type::ObjectRef;
    return obj;
}

OpaquePointer::OpaquePointer(const Napi::CallbackInfo& info) : Napi::ObjectWrap<OpaquePointer>{ info } {}

OpaquePointer::~OpaquePointer() {
    releaseReal();
}

Napi::Value OpaquePointer::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<OpaquePointer>(info.Env());
}

Napi::Value OpaquePointer::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value OpaquePointer::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value OpaquePointer::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value OpaquePointer::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == OpaquePointer::get(info[0]));
}

Napi::Value OpaquePointer::fromPointer(const Napi::CallbackInfo& info) {
    return create(info.Env(), reinterpret_cast<void*>(asU64(info[0])));
}

Napi::Value OpaquePointer::fromObject(const Napi::CallbackInfo& info) {
    return create(info.Env(), valueAsObject(info[0]));
}

Napi::Value OpaquePointer::getRawPointer(const Napi::CallbackInfo& info) {
    return fromU64(info.Env(), reinterpret_cast<std::uint64_t>(m_val));
}

Napi::Value OpaquePointer::asPointer(const Napi::CallbackInfo& info) {
    return m_val && m_type == Type::Raw ? fromU64(info.Env(), reinterpret_cast<std::uint64_t>(m_val)) : info.Env().Undefined();
}

Napi::Value OpaquePointer::asObject(const Napi::CallbackInfo& info) {
    return m_val && m_type == Type::ObjectRef ? static_cast<Napi::ObjectReference*>(m_val)->Value() : info.Env().Undefined();
}

void OpaquePointer::releaseReal() {
    if (m_val && m_type == Type::ObjectRef)
        delete static_cast<Napi::ObjectReference*>(m_val);
    m_val = nullptr;
}

Napi::Value OpaquePointer::release(const Napi::CallbackInfo& info) {
    releaseReal();
    return info.Env().Undefined();
}

}
