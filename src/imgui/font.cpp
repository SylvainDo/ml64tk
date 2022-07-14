#include "core/type/instanceOf.hpp"
#include "imgui/convert.hpp"
#include "imgui/drawList.hpp"
#include "imgui/font.hpp"
#include "imgui/fontAtlas.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core::type::convert;

namespace imgui {

Napi::FunctionReference Font::m_ctor;

Napi::Object Font::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "Font";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&Font::getTypeId>("typeId"),
        InstanceAccessor<&Font::toDebugString>("debugString"),
        InstanceMethod<&Font::ref>("ref"),
        InstanceMethod<&Font::unref>("unref"),

        InstanceMethod<&Font::equals>("equals"),
        InstanceAccessor<&Font::getFallbackAdvanceX>("fallbackAdvanceX"),
        InstanceAccessor<&Font::getFontSize>("fontSize"),
        InstanceAccessor<&Font::getContainerAtlas>("containerAtlas"),
        InstanceAccessor<&Font::getScale, &Font::setScale>("scale"),
        InstanceAccessor<&Font::getAscent>("ascent"),
        InstanceAccessor<&Font::getDescent>("descent"),
        InstanceAccessor<&Font::getMetricsTotalSurface>("metricsTotalSurface"),
        InstanceMethod<&Font::getCharAdvance>("getCharAdvance"),
        InstanceMethod<&Font::calcTextSizeA>("calcTextSizeA"),
        InstanceMethod<&Font::calcWordWrapPositionA>("calcWordWrapPositionA"),
        InstanceMethod<&Font::renderChar>("renderChar"),
        InstanceMethod<&Font::renderText>("renderText")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object Font::create(Napi::Env env, ImFont* val) {
    const auto obj = m_ctor.New({});
    unwrap(obj)->m_val = val;
    return obj;
}

Font::Font(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Font>{ info } {}

Napi::Value Font::getTypeId(const Napi::CallbackInfo& info) {
    return core::type::fromTypeId<Font>(info.Env());
}

Napi::Value Font::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value Font::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value Font::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value Font::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == Font::get(info[0]));
}

Napi::Value Font::getFallbackAdvanceX(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->FallbackAdvanceX);
}

Napi::Value Font::getFontSize(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->FontSize);
}

Napi::Value Font::getContainerAtlas(const Napi::CallbackInfo& info) {
    return FontAtlas::create(info.Env(), m_val->ContainerAtlas);
}

Napi::Value Font::getScale(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->Scale);
}

void Font::setScale(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->Scale = asF32(val);
}

Napi::Value Font::getAscent(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->Ascent);
}

Napi::Value Font::getDescent(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->Descent);
}

Napi::Value Font::getMetricsTotalSurface(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->MetricsTotalSurface);
}

Napi::Value Font::getCharAdvance(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->GetCharAdvance(/* c */ asU16(info[0])));
}

Napi::Value Font::calcTextSizeA(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->CalcTextSizeA(
        /* size */ asF32(info[0]),
        /* max_width */ asF32(info[1]),
        /* wrap_width */ asF32(info[2]),
        /* text */ asStrUtf8(info[3]).c_str(), nullptr));
}

Napi::Value Font::calcWordWrapPositionA(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), m_val->CalcWordWrapPositionA(
        /* scale */ asF32(info[0]),
        /* text */ asStrUtf8(info[1]).c_str(), nullptr,
        /* wrap_width */ asF32(info[2])));
}

Napi::Value Font::renderChar(const Napi::CallbackInfo& info) {
    m_val->RenderChar(
        /* draw_list */ DrawList::get(info[0]),
        /* size */ asF32(info[1]),
        /* pos */ asVec2(info[2]),
        /* col */ asColor(info[3]),
        /* c */ asU16(info[4]));
    return info.Env().Undefined();
}

Napi::Value Font::renderText(const Napi::CallbackInfo& info) {
    m_val->RenderText(
        /* draw_list */ DrawList::get(info[0]),
        /* size */ asF32(info[1]),
        /* pos */ asVec2(info[2]),
        /* col */ asColor(info[3]),
        /* clip_rect */ asVec4(info[4]),
        /* text */ asStrUtf8(info[5]).c_str(), nullptr,
        /* wrap_width */ asF32Or(info[6], 0.0f),
        /* cpu_fine_clip */ asBoolOr(info[7], false));
    return info.Env().Undefined();
}

}
