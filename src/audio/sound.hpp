#pragma once

#include <napi.h>
#include <SFML/Audio/Sound.hpp>

namespace audio {

class Sound final : public Napi::ObjectWrap<Sound> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);

    static Sound* unwrap(const Napi::Object& obj) {
        return Sound::Unwrap(obj);
    }

    static Sound* unwrap(const Napi::Value& val) {
        return Sound::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static sf::Sound* get(const T& val) {
        return &unwrap(val)->m_inst;
    }

    sf::Sound* get() {
        return &m_inst;
    }

    Sound(const Napi::CallbackInfo& info);
    virtual ~Sound() = default;

private:
    static Napi::FunctionReference m_ctor;
    sf::Sound m_inst;
    Napi::ObjectReference m_buf;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    /* SoundSource */
    Napi::Value getPitch(const Napi::CallbackInfo& info);
    void setPitch(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getVolume(const Napi::CallbackInfo& info);
    void setVolume(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getPosition(const Napi::CallbackInfo& info);
    void setPosition(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getRelativeToListener(const Napi::CallbackInfo& info);
    void setRelativeToListener(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getMinDistance(const Napi::CallbackInfo& info);
    void setMinDistance(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getAttenuation(const Napi::CallbackInfo& info);
    void setAttenuation(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value play(const Napi::CallbackInfo& info);
    Napi::Value pause(const Napi::CallbackInfo& info);
    Napi::Value stop(const Napi::CallbackInfo& info);
    Napi::Value getStatus(const Napi::CallbackInfo& info);
    /* Sound */
    Napi::Value getBuffer(const Napi::CallbackInfo& info);
    void setBuffer(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getLoop(const Napi::CallbackInfo& info);
    void setLoop(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getPlayingOffset(const Napi::CallbackInfo& info);
    void setPlayingOffset(const Napi::CallbackInfo& info, const Napi::Value& val);
};

}
