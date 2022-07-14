#include "core/type/instanceOf.hpp"
#include "imgui/keyData.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core::type::convert;

namespace imgui {

Napi::FunctionReference KeyData::m_ctor;

Napi::Object KeyData::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "KeyData";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&KeyData::getTypeId>("typeId"),
        InstanceAccessor<&KeyData::toDebugString>("debugString"),
        InstanceMethod<&KeyData::ref>("ref"),
        InstanceMethod<&KeyData::unref>("unref"),

        InstanceMethod<&KeyData::equals>("equals"),
        InstanceAccessor<&KeyData::getDown, &KeyData::setDown>("down"),
        InstanceAccessor<&KeyData::getDownDuration, &KeyData::setDownDuration>("downDuration"),
        InstanceAccessor<&KeyData::getDownDurationPrev, &KeyData::setDownDurationPrev>("downDurationPrev"),
        InstanceAccessor<&KeyData::getAnalogValue, &KeyData::setAnalogValue>("analogValue")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object KeyData::create(Napi::Env, ImGuiKeyData* val) {
    const auto obj = m_ctor.New({ m_ctor.Value() });
    unwrap(obj)->m_val = val;
    return obj;
}

KeyData::KeyData(const Napi::CallbackInfo& info) : Napi::ObjectWrap<KeyData>{ info } {}

Napi::Value KeyData::getTypeId(const Napi::CallbackInfo& info) {
    return core::type::fromTypeId<KeyData>(info.Env());
}

Napi::Value KeyData::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value KeyData::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value KeyData::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value KeyData::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == KeyData::get(info[0]));
}

Napi::Value KeyData::getDown(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->Down);
}

void KeyData::setDown(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->Down = asBool(val);
}

Napi::Value KeyData::getDownDuration(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->DownDuration);
}

void KeyData::setDownDuration(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DownDuration = asF32(val);
}

Napi::Value KeyData::getDownDurationPrev(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->DownDurationPrev);
}

void KeyData::setDownDurationPrev(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DownDurationPrev = asF32(val);
}

Napi::Value KeyData::getAnalogValue(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->AnalogValue);
}

void KeyData::setAnalogValue(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->AnalogValue = asF32(val);
}

}
