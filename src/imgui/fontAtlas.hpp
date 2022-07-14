#pragma once

#include <imgui.h>
#include <napi.h>

namespace imgui {

class FontAtlas final : public Napi::ObjectWrap<FontAtlas> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env, ImFontAtlas* val);

    static FontAtlas* unwrap(const Napi::Object& obj) {
        return FontAtlas::Unwrap(obj);
    }

    static FontAtlas* unwrap(const Napi::Value& val) {
        return FontAtlas::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImFontAtlas* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImFontAtlas* get() {
        return m_val;
    }

    FontAtlas(const Napi::CallbackInfo& info);
    virtual ~FontAtlas() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImFontAtlas* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value addFontDefault(const Napi::CallbackInfo& info);
    Napi::Value addFontFromFile(const Napi::CallbackInfo& info);
    Napi::Value addFontFromMemory(const Napi::CallbackInfo& info);
    Napi::Value clear(const Napi::CallbackInfo& info);
    static Napi::Value getGlyphRanges(const Napi::CallbackInfo& info, const ImWchar* v);
    Napi::Value getGlyphRangesDefault(const Napi::CallbackInfo& info);
    Napi::Value getGlyphRangesKorean(const Napi::CallbackInfo& info);
    Napi::Value getGlyphRangesJapanese(const Napi::CallbackInfo& info);
    Napi::Value getGlyphRangesChineseFull(const Napi::CallbackInfo& info);
    Napi::Value getGlyphRangesChineseSimplifiedCommon(const Napi::CallbackInfo& info);
    Napi::Value getGlyphRangesCyrillic(const Napi::CallbackInfo& info);
    Napi::Value getGlyphRangesThai(const Napi::CallbackInfo& info);
    Napi::Value getGlyphRangesVietnamese(const Napi::CallbackInfo& info);
};

}
