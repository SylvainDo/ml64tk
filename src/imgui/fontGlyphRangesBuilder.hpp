#pragma once

#include <imgui.h>
#include <napi.h>

namespace imgui {

class FontGlyphRangesBuilder final : public Napi::ObjectWrap<FontGlyphRangesBuilder> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env);

    static FontGlyphRangesBuilder* unwrap(const Napi::Object& obj) {
        return FontGlyphRangesBuilder::Unwrap(obj);
    }

    static FontGlyphRangesBuilder* unwrap(const Napi::Value& val) {
        return FontGlyphRangesBuilder::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImFontGlyphRangesBuilder* get(const T& val) {
        return &unwrap(val)->m_val;
    }

    ImFontGlyphRangesBuilder* get() {
        return &m_val;
    }

    FontGlyphRangesBuilder(const Napi::CallbackInfo& info);
    virtual ~FontGlyphRangesBuilder() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImFontGlyphRangesBuilder m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value clear(const Napi::CallbackInfo& info);
    Napi::Value getBit(const Napi::CallbackInfo& info);
    Napi::Value setBit(const Napi::CallbackInfo& info);
    Napi::Value addChar(const Napi::CallbackInfo& info);
    Napi::Value addText(const Napi::CallbackInfo& info);
    Napi::Value addRanges(const Napi::CallbackInfo& info);
    Napi::Value buildRanges(const Napi::CallbackInfo& info);
};

}
