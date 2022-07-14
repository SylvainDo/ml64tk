#pragma once

#include <napi.h>

struct ImFont;

namespace imgui {

class Font final : public Napi::ObjectWrap<Font> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env env, ImFont* val);

    static Font* unwrap(const Napi::Object& obj) {
        return Font::Unwrap(obj);
    }

    static Font* unwrap(const Napi::Value& val) {
        return Font::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImFont* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImFont* get() {
        return m_val;
    }

    Font(const Napi::CallbackInfo& info);
    virtual ~Font() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImFont* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getFallbackAdvanceX(const Napi::CallbackInfo& info);
    Napi::Value getFontSize(const Napi::CallbackInfo& info);
    Napi::Value getContainerAtlas(const Napi::CallbackInfo& info);
    Napi::Value getScale(const Napi::CallbackInfo& info);
    void setScale(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getAscent(const Napi::CallbackInfo& info);
    Napi::Value getDescent(const Napi::CallbackInfo& info);
    Napi::Value getMetricsTotalSurface(const Napi::CallbackInfo& info);
    Napi::Value getCharAdvance(const Napi::CallbackInfo& info);
    Napi::Value calcTextSizeA(const Napi::CallbackInfo& info);
    Napi::Value calcWordWrapPositionA(const Napi::CallbackInfo& info);
    Napi::Value renderChar(const Napi::CallbackInfo& info);
    Napi::Value renderText(const Napi::CallbackInfo& info);
};

}
