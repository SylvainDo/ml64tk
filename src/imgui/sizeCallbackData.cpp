#include "core/instanceOf.hpp"
#include "imgui/convert.hpp"
#include "imgui/sizeCallbackData.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core;

namespace imgui {

Napi::FunctionReference SizeCallbackData::m_ctor;

Napi::Object SizeCallbackData::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "SizeCallbackData";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&SizeCallbackData::getTypeId>("typeId"),
        InstanceAccessor<&SizeCallbackData::toDebugString>("debugString"),
        InstanceMethod<&SizeCallbackData::ref>("ref"),
        InstanceMethod<&SizeCallbackData::unref>("unref"),

        InstanceMethod<&SizeCallbackData::equals>("equals"),
        InstanceAccessor<&SizeCallbackData::getPos>("pos"),
        InstanceAccessor<&SizeCallbackData::getCurrentSize>("currentSize"),
        InstanceAccessor<&SizeCallbackData::getDesiredSize, &SizeCallbackData::setDesiredSize>("desiredSize")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object SizeCallbackData::create(Napi::Env, ImGuiSizeCallbackData* val) {
    const auto obj = m_ctor.New({ m_ctor.Value() });
    unwrap(obj)->m_val = val;
    return obj;
}

SizeCallbackData::SizeCallbackData(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SizeCallbackData>{ info } {}

Napi::Value SizeCallbackData::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<SizeCallbackData>(info.Env());
}

Napi::Value SizeCallbackData::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("ImGui.SizeCallbackData (this={}; instance={})",
        fmt::ptr(this), fmt::ptr(m_val)));
}

Napi::Value SizeCallbackData::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value SizeCallbackData::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value SizeCallbackData::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == SizeCallbackData::get(info[0]));
}

Napi::Value SizeCallbackData::getPos(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->Pos);
}

Napi::Value SizeCallbackData::getCurrentSize(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->CurrentSize);
}

Napi::Value SizeCallbackData::getDesiredSize(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->DesiredSize);
}

void SizeCallbackData::setDesiredSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DesiredSize = asVec2(val);
}

}
