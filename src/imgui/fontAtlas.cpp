#include "core/instanceOf.hpp"
#include "imgui/font.hpp"
#include "imgui/fontAtlas.hpp"
#include "imgui/fontConfig.hpp"

#include <fmt/format.h>

using namespace core;

namespace imgui {

Napi::FunctionReference FontAtlas::m_ctor;

Napi::Object FontAtlas::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "FontAtlas";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&FontAtlas::getTypeId>("typeId"),
        InstanceAccessor<&FontAtlas::toDebugString>("debugString"),
        InstanceMethod<&FontAtlas::ref>("ref"),
        InstanceMethod<&FontAtlas::unref>("unref"),

        InstanceMethod<&FontAtlas::equals>("equals"),
        InstanceMethod<&FontAtlas::addFontDefault>("addFontDefault"),
        InstanceMethod<&FontAtlas::addFontFromFile>("addFontFromFile"),
        InstanceMethod<&FontAtlas::addFontFromMemory>("addFontFromMemory"),
        InstanceMethod<&FontAtlas::clear>("clear"),
        InstanceAccessor<&FontAtlas::getGlyphRangesDefault>("glyphRangesDefault"),
        InstanceAccessor<&FontAtlas::getGlyphRangesKorean>("glyphRangesKorean"),
        InstanceAccessor<&FontAtlas::getGlyphRangesJapanese>("glyphRangesJapanese"),
        InstanceAccessor<&FontAtlas::getGlyphRangesChineseFull>("glyphRangesChineseFull"),
        InstanceAccessor<&FontAtlas::getGlyphRangesChineseSimplifiedCommon>("glyphRangesChineseSimplifiedCommon"),
        InstanceAccessor<&FontAtlas::getGlyphRangesCyrillic>("glyphRangesCyrillic"),
        InstanceAccessor<&FontAtlas::getGlyphRangesThai>("glyphRangesThai"),
        InstanceAccessor<&FontAtlas::getGlyphRangesVietnamese>("glyphRangesVietnamese")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object FontAtlas::create(Napi::Env, ImFontAtlas* val) {
    const auto obj = m_ctor.New({});
    unwrap(obj)->m_val = val;
    return obj;
}

FontAtlas::FontAtlas(const Napi::CallbackInfo& info) : Napi::ObjectWrap<FontAtlas>{ info } {}

Napi::Value FontAtlas::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<FontAtlas>(info.Env());
}

Napi::Value FontAtlas::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value FontAtlas::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value FontAtlas::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value FontAtlas::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == FontAtlas::get(info[0]));
}

Napi::Value FontAtlas::addFontDefault(const Napi::CallbackInfo& info) {
    return Font::create(info.Env(), m_val->AddFontDefault(
        /* font_cfg */ info[0].IsUndefined() ? nullptr : FontConfig::get(info[0])));
}

Napi::Value FontAtlas::addFontFromFile(const Napi::CallbackInfo& info) {
    return Font::create(info.Env(),m_val->AddFontFromFileTTF(
        /* filename */ asStrUtf8(info[0]).c_str(),
        /* size_pixels */ asF32(info[1]),
        /* font_cfg */ info[2].IsUndefined() ? nullptr : FontConfig::get(info[2]),
        /* glyph_ranges */ info[3].IsUndefined() ? nullptr : info[3].As<Napi::Uint16Array>().Data()));
}

Napi::Value FontAtlas::addFontFromMemory(const Napi::CallbackInfo& info) {
    static ImFontConfig fontConfigDefault;
    auto fontConfig = info[2].IsUndefined() ? &fontConfigDefault : FontConfig::get(info[2]);
    fontConfig->FontDataOwnedByAtlas = false;
    auto buf = info[0].As<Napi::Uint8Array>();
    return Font::create(info.Env(), m_val->AddFontFromMemoryTTF(
        /* font_data */ buf.Data(),
        /* font_size */ buf.ByteLength(),
        /* size_pixels */ asF32(info[1]),
        /* font_cfg */ fontConfig,
        /* glyph_ranges */ info[3].IsUndefined() ? nullptr : info[3].As<Napi::Uint16Array>().Data()));
}

Napi::Value FontAtlas::clear(const Napi::CallbackInfo& info) {
    m_val->Clear();
    return info.Env().Undefined();
}

Napi::Value FontAtlas::getGlyphRanges(const Napi::CallbackInfo& info, const ImWchar* v) {
    int size{}; while (v[size++]);
    auto ranges = Napi::Uint16Array::New(info.Env(), size);
    std::memcpy(ranges.Data(), v, sizeof(ImWchar) * size);
    return ranges;
}

Napi::Value FontAtlas::getGlyphRangesDefault(const Napi::CallbackInfo& info) {
    return getGlyphRanges(info, m_val->GetGlyphRangesDefault());
}

Napi::Value FontAtlas::getGlyphRangesKorean(const Napi::CallbackInfo& info) {
    return getGlyphRanges(info, m_val->GetGlyphRangesKorean());
}

Napi::Value FontAtlas::getGlyphRangesJapanese(const Napi::CallbackInfo& info) {
    return getGlyphRanges(info, m_val->GetGlyphRangesJapanese());
}

Napi::Value FontAtlas::getGlyphRangesChineseFull(const Napi::CallbackInfo& info) {
    return getGlyphRanges(info, m_val->GetGlyphRangesChineseFull());
}

Napi::Value FontAtlas::getGlyphRangesChineseSimplifiedCommon(const Napi::CallbackInfo& info) {
    return getGlyphRanges(info, m_val->GetGlyphRangesChineseSimplifiedCommon());
}

Napi::Value FontAtlas::getGlyphRangesCyrillic(const Napi::CallbackInfo& info) {
    return getGlyphRanges(info, m_val->GetGlyphRangesCyrillic());
}

Napi::Value FontAtlas::getGlyphRangesThai(const Napi::CallbackInfo& info) {
    return getGlyphRanges(info, m_val->GetGlyphRangesThai());
}

Napi::Value FontAtlas::getGlyphRangesVietnamese(const Napi::CallbackInfo& info) {
    return getGlyphRanges(info, m_val->GetGlyphRangesVietnamese());
}

}
