#include "core/instanceOf.hpp"
#include "gfx/font.hpp"
#include "gfx/texture.hpp"
#include "imgui/convert.hpp"

#include <fmt/format.h>

using namespace core;
using namespace imgui;

namespace gfx {

Napi::FunctionReference Font::m_ctor;

Napi::Object Font::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "Font";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&Font::getTypeId>("typeId"),
        InstanceAccessor<&Font::toDebugString>("debugString"),
        InstanceMethod<&Font::ref>("ref"),
        InstanceMethod<&Font::unref>("unref"),

        InstanceMethod<&Font::equals>("equals"),
        InstanceMethod<&Font::loadFromFile>("loadFromFile"),
        InstanceMethod<&Font::loadFromMemory>("loadFromMemory"),
        InstanceAccessor<&Font::getFillColor, &Font::setFillColor>("fillColor"),
        InstanceAccessor<&Font::getOutlineColor, &Font::setOutlineColor>("outlineColor"),
        InstanceAccessor<&Font::getOutlineThickness, &Font::setOutlineThickness>("outlineThickness"),
        InstanceMethod<&Font::renderText>("renderText")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Font::Font(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Font>{ info } {}

Napi::Value Font::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<Font>(info.Env());
}

inline std::string formatColor(const ImColor& v) {
    return fmt::format("({},{},{},{})", v.Value.x, v.Value.y, v.Value.z, v.Value.w);
}

Napi::Value Font::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("Font (font={}; fillColor={}; outlineColor={}; outlineThickness={})",
        fmt::ptr(m_font.get()), formatColor(m_fillColor), formatColor(m_outlineColor), m_outlineThickness));
}

Napi::Value Font::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value Font::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value Font::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), this == Font::unwrap(info[0]));
}

Napi::Value Font::loadFromFile(const Napi::CallbackInfo& info) {
    const auto filename = asStrUtf8(info[0]);
    m_font.reset(TTF_OpenFont(filename.c_str(), /* ptsize */ asS32(info[1])));
    if (!m_font) throw Napi::Error::New(info.Env(), fmt::format("failed to load font from file `{}`: {}", filename, TTF_GetError()));
    return info.Env().Undefined();
}

Napi::Value Font::loadFromMemory(const Napi::CallbackInfo& info) {
    auto data = info[0].As<Napi::Uint8Array>();
    m_font.reset(TTF_OpenFontRW(SDL_RWFromMem(data.Data(), data.ByteLength()), true, /* ptsize */ asS32(info[1])));
    if (!m_font) throw Napi::Error::New(info.Env(), fmt::format("failed to load font from memory: {}", TTF_GetError()));
    return info.Env().Undefined();
}

Napi::Value Font::getFillColor(const Napi::CallbackInfo& info) {
    return fromColor(info.Env(), m_fillColor);
}

void Font::setFillColor(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_fillColor = asColor(val);
}

Napi::Value Font::getOutlineColor(const Napi::CallbackInfo& info) {
    return fromColor(info.Env(), m_outlineColor);
}

void Font::setOutlineColor(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_outlineColor = asColor(val);
}

Napi::Value Font::getOutlineThickness(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_outlineThickness);
}

void Font::setOutlineThickness(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_outlineThickness = asS32(val);
}

inline SDL_Color convertColor(const ImColor& v) {
    return { static_cast<Uint8>(v.Value.x * 255), static_cast<Uint8>(v.Value.y * 255),
        static_cast<Uint8>(v.Value.z * 255), static_cast<Uint8>(v.Value.w * 255) };
}

Napi::Value Font::renderText(const Napi::CallbackInfo& info) {
    const auto text = asStrUtf8(info[0]);
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> bg{ nullptr, SDL_FreeSurface };
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> fg{ nullptr, SDL_FreeSurface };

    TTF_SetFontOutline(m_font.get(), m_outlineThickness);
    bg.reset(TTF_RenderUTF8_Blended(m_font.get(), text.c_str(), convertColor(m_outlineColor)));
    TTF_SetFontOutline(m_font.get(), 0);
    fg.reset(TTF_RenderUTF8_Blended(m_font.get(), text.c_str(), convertColor(m_fillColor)));

    SDL_Rect dst{ m_outlineThickness, m_outlineThickness, fg->w, fg->h };
    SDL_BlitSurface(fg.get(), nullptr, bg.get(), &dst);
    bg.reset(SDL_ConvertSurfaceFormat(bg.get(), SDL_PIXELFORMAT_RGBA32, 0));

    return Texture::create(info.Env(), bg->w, bg->h, bg->pixels);
}

}
