#pragma once

#include <memory>

#include <imgui.h>
#include <napi.h>
#include <SDL_ttf.h>

namespace gfx {

class Font final : public Napi::ObjectWrap<Font> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);

    static Font* unwrap(const Napi::Object& obj) {
        return Font::Unwrap(obj);
    }

    static Font* unwrap(const Napi::Value& val) {
        return Font::Unwrap(val.As<Napi::Object>());
    }

    Font(const Napi::CallbackInfo& info);
    virtual ~Font() = default;

private:
    static Napi::FunctionReference m_ctor;
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> m_font{ nullptr, TTF_CloseFont };
    ImColor m_fillColor{ IM_COL32_WHITE };
    ImColor m_outlineColor{ IM_COL32_WHITE };
    int m_outlineThickness{};

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value loadFromFile(const Napi::CallbackInfo& info);
    Napi::Value loadFromMemory(const Napi::CallbackInfo& info);
    Napi::Value getFillColor(const Napi::CallbackInfo& info);
    void setFillColor(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOutlineColor(const Napi::CallbackInfo& info);
    void setOutlineColor(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOutlineThickness(const Napi::CallbackInfo& info);
    void setOutlineThickness(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value renderText(const Napi::CallbackInfo& info);
};

}
