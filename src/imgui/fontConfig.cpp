#include "core/instanceOf.hpp"
#include "imgui/convert.hpp"
#include "imgui/fontConfig.hpp"

#include <fmt/format.h>

using namespace core;

namespace imgui {

Napi::FunctionReference FontConfig::m_ctor;

Napi::Object FontConfig::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "FontConfig";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&FontConfig::getTypeId>("typeId"),
        InstanceAccessor<&FontConfig::toDebugString>("debugString"),
        InstanceMethod<&FontConfig::ref>("ref"),
        InstanceMethod<&FontConfig::unref>("unref"),

        InstanceMethod<&FontConfig::equals>("equals"),
        InstanceAccessor<&FontConfig::getFontNo, &FontConfig::setFontNo>("fontNo"),
        InstanceAccessor<&FontConfig::getSizePixels, &FontConfig::setSizePixels>("sizePixels"),
        InstanceAccessor<&FontConfig::getOversampleH, &FontConfig::setOversampleH>("oversampleH"),
        InstanceAccessor<&FontConfig::getOversampleV, &FontConfig::setOversampleV>("oversampleV"),
        InstanceAccessor<&FontConfig::getPixelSnapH, &FontConfig::setPixelSnapH>("pixelSnapH"),
        InstanceAccessor<&FontConfig::getGlyphExtraSpacing, &FontConfig::setGlyphExtraSpacing>("glyphExtraSpacing"),
        InstanceAccessor<&FontConfig::getGlyphOffset, &FontConfig::setGlyphOffset>("glyphOffset"),
        InstanceAccessor<&FontConfig::getGlyphMinAdvanceX, &FontConfig::setGlyphMinAdvanceX>("glyphMinAdvanceX"),
        InstanceAccessor<&FontConfig::getGlyphMaxAdvanceX, &FontConfig::setGlyphMaxAdvanceX>("glyphMaxAdvanceX"),
        InstanceAccessor<&FontConfig::getMergeMode, &FontConfig::setMergeMode>("mergeMode"),
        InstanceAccessor<&FontConfig::getFontBuilderFlags, &FontConfig::setFontBuilderFlags>("fontBuilderFlags"),
        InstanceAccessor<&FontConfig::getRasterizerMultiply, &FontConfig::setRasterizerMultiply>("rasterizerMultiply"),
        InstanceAccessor<&FontConfig::getEllipsisChar, &FontConfig::setEllipsisChar>("ellipsisChar")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object FontConfig::create(Napi::Env) {
    return m_ctor.New({});
}

FontConfig::FontConfig(const Napi::CallbackInfo& info) : Napi::ObjectWrap<FontConfig>{ info } {}

Napi::Value FontConfig::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<FontConfig>(info.Env());
}

Napi::Value FontConfig::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("ImGui.FontConfig (this={}; instance={})",
        fmt::ptr(this), fmt::ptr(&m_val)));
}

Napi::Value FontConfig::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value FontConfig::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value FontConfig::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), &m_val == FontConfig::get(info[0]));
}

Napi::Value FontConfig::getFontNo(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val.FontNo);
}

void FontConfig::setFontNo(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.FontNo = asS32(val);
}

Napi::Value FontConfig::getSizePixels(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val.SizePixels);
}

void FontConfig::setSizePixels(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.SizePixels = asF32(val);
}

Napi::Value FontConfig::getOversampleH(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val.OversampleH);
}

void FontConfig::setOversampleH(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.OversampleH = asS32(val);
}

Napi::Value FontConfig::getOversampleV(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val.OversampleV);
}

void FontConfig::setOversampleV(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.OversampleV = asS32(val);
}

Napi::Value FontConfig::getPixelSnapH(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val.PixelSnapH);
}

void FontConfig::setPixelSnapH(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.PixelSnapH = asBool(val);
}

Napi::Value FontConfig::getGlyphExtraSpacing(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val.GlyphExtraSpacing);
}

void FontConfig::setGlyphExtraSpacing(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.GlyphExtraSpacing = asVec2(val);
}

Napi::Value FontConfig::getGlyphOffset(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val.GlyphOffset);
}

void FontConfig::setGlyphOffset(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.GlyphOffset = asVec2(val);
}

Napi::Value FontConfig::getGlyphMinAdvanceX(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val.GlyphMinAdvanceX);
}

void FontConfig::setGlyphMinAdvanceX(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.GlyphMinAdvanceX = asF32(val);
}

Napi::Value FontConfig::getGlyphMaxAdvanceX(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val.GlyphMaxAdvanceX);
}

void FontConfig::setGlyphMaxAdvanceX(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.GlyphMaxAdvanceX = asF32(val);
}

Napi::Value FontConfig::getMergeMode(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val.MergeMode);
}

void FontConfig::setMergeMode(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.MergeMode = asBool(val);
}

Napi::Value FontConfig::getFontBuilderFlags(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_val.FontBuilderFlags);
}

void FontConfig::setFontBuilderFlags(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.FontBuilderFlags = asU32(val);
}

Napi::Value FontConfig::getRasterizerMultiply(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val.RasterizerMultiply);
}

void FontConfig::setRasterizerMultiply(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.RasterizerMultiply = asF32(val);
}

Napi::Value FontConfig::getEllipsisChar(const Napi::CallbackInfo& info) {
    return fromU16(info.Env(), m_val.EllipsisChar);
}

void FontConfig::setEllipsisChar(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val.EllipsisChar = asU16(val);
}

}
