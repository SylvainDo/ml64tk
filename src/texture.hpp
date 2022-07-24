#pragma once

#include <napi.h>

class Texture final : public Napi::ObjectWrap<Texture> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);

    static Texture* unwrap(const Napi::Object& obj) {
        return Texture::Unwrap(obj);
    }

    static Texture* unwrap(const Napi::Value& val) {
        return Texture::Unwrap(val.As<Napi::Object>());
    }

    unsigned getId() const {
        return m_id;
    }

    void* getIdPtr() const;

    int getWidth() const {
        return m_width;
    }

    int getHeight() const {
        return m_height;
    }

    Texture(const Napi::CallbackInfo& info);
    virtual ~Texture() = default;

private:
    static Napi::FunctionReference m_ctor;
    unsigned m_id;
    int m_width{}, m_height{};

    void load(int width, int height, void* pixels);

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value loadFromFile(const Napi::CallbackInfo& info);
    Napi::Value loadFromMemory(const Napi::CallbackInfo& info);
    Napi::Value loadFromRGBA32(const Napi::CallbackInfo& info);
    Napi::Value getId(const Napi::CallbackInfo& info);
    Napi::Value getSize(const Napi::CallbackInfo& info);
};
