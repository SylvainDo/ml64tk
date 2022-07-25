#pragma once

#include <napi.h>
#include <SFML/Audio/SoundBuffer.hpp>

namespace audio {

class SoundBuffer final : public Napi::ObjectWrap<SoundBuffer> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);

    static SoundBuffer* unwrap(const Napi::Object& obj) {
        return SoundBuffer::Unwrap(obj);
    }

    static SoundBuffer* unwrap(const Napi::Value& val) {
        return SoundBuffer::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static sf::SoundBuffer* get(const T& val) {
        return &unwrap(val)->m_inst;
    }

    sf::SoundBuffer* get() {
        return &m_inst;
    }

    SoundBuffer(const Napi::CallbackInfo& info);
    virtual ~SoundBuffer() = default;

private:
    static Napi::FunctionReference m_ctor;
    sf::SoundBuffer m_inst;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value loadFromFile(const Napi::CallbackInfo& info);
    Napi::Value loadFromMemory(const Napi::CallbackInfo& info);
    Napi::Value loadFromSamples(const Napi::CallbackInfo& info);
    Napi::Value saveToFile(const Napi::CallbackInfo& info);
    Napi::Value getSamples(const Napi::CallbackInfo& info);
    Napi::Value getSampleCount(const Napi::CallbackInfo& info);
    Napi::Value getSampleRate(const Napi::CallbackInfo& info);
    Napi::Value getChannelCount(const Napi::CallbackInfo& info);
    Napi::Value getDuration(const Napi::CallbackInfo& info);
};

}
