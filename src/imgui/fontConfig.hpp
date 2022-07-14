#pragma once

#include <imgui.h>
#include <napi.h>

namespace imgui {

class FontConfig final : public Napi::ObjectWrap<FontConfig> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env env);

    static FontConfig* unwrap(const Napi::Object& obj) {
        return FontConfig::Unwrap(obj);
    }

    static FontConfig* unwrap(const Napi::Value& val) {
        return FontConfig::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImFontConfig* get(const T& val) {
        return &unwrap(val)->m_val;
    }

    ImFontConfig* get() {
        return &m_val;
    }

    FontConfig(const Napi::CallbackInfo& info);
    virtual ~FontConfig() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImFontConfig m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getFontNo(const Napi::CallbackInfo& info);
    void setFontNo(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getSizePixels(const Napi::CallbackInfo& info);
    void setSizePixels(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getOversampleH(const Napi::CallbackInfo& info);
    void setOversampleH(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getOversampleV(const Napi::CallbackInfo& info);
    void setOversampleV(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getPixelSnapH(const Napi::CallbackInfo& info);
    void setPixelSnapH(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getGlyphExtraSpacing(const Napi::CallbackInfo& info);
    void setGlyphExtraSpacing(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getGlyphOffset(const Napi::CallbackInfo& info);
    void setGlyphOffset(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getGlyphMinAdvanceX(const Napi::CallbackInfo& info);
    void setGlyphMinAdvanceX(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getGlyphMaxAdvanceX(const Napi::CallbackInfo& info);
    void setGlyphMaxAdvanceX(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMergeMode(const Napi::CallbackInfo& info);
    void setMergeMode(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFontBuilderFlags(const Napi::CallbackInfo& info);
    void setFontBuilderFlags(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getRasterizerMultiply(const Napi::CallbackInfo& info);
    void setRasterizerMultiply(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getEllipsisChar(const Napi::CallbackInfo& info);
    void setEllipsisChar(const Napi::CallbackInfo&, const Napi::Value& val);
};

}
