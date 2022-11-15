#pragma once

#include <optional>

#include <napi.h>

namespace gfx {

class Texture final : public Napi::ObjectWrap<Texture> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env env, int width, int height, void* pixels);

    static Texture* unwrap(const Napi::Object& obj) {
        return Texture::Unwrap(obj);
    }

    static Texture* unwrap(const Napi::Value& val) {
        return Texture::Unwrap(val.As<Napi::Object>());
    }

    unsigned getId() const {
        return m_id.value_or(0);
    }

    void* getIdPtr() const;

    int getWidth() const {
        return m_width;
    }

    int getHeight() const {
        return m_height;
    }

    Texture(const Napi::CallbackInfo& info);
    virtual ~Texture();

private:
    static Napi::FunctionReference m_ctor;
    std::optional<unsigned> m_id;
    int m_width{}, m_height{};

    void create();
    void load(int width, int height, void* pixels, unsigned format);

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value loadFromFile(const Napi::CallbackInfo& info);
    Napi::Value loadFromMemory(const Napi::CallbackInfo& info);
    Napi::Value loadFromRGBA32(const Napi::CallbackInfo& info);
    Napi::Value loadFromBGRA32(const Napi::CallbackInfo& info);
    Napi::Value getId(const Napi::CallbackInfo& info);
    Napi::Value getSize(const Napi::CallbackInfo& info);
};

}
