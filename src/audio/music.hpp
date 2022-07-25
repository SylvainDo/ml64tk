#pragma once

#include <napi.h>
#include <SFML/Audio/Music.hpp>

namespace audio {

class Music final : public Napi::ObjectWrap<Music> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);

    static Music* unwrap(const Napi::Object& obj) {
        return Music::Unwrap(obj);
    }

    static Music* unwrap(const Napi::Value& val) {
        return Music::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static sf::Music* get(const T& val) {
        return &unwrap(val)->m_inst;
    }

    sf::Music* get() {
        return &m_inst;
    }

    Music(const Napi::CallbackInfo& info);
    virtual ~Music() = default;

private:
    static Napi::FunctionReference m_ctor;
    sf::Music m_inst;
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
    /* SoundStream */
    Napi::Value getChannelCount(const Napi::CallbackInfo& info);
    Napi::Value getSampleRate(const Napi::CallbackInfo& info);
    Napi::Value getPlayingOffset(const Napi::CallbackInfo& info);
    void setPlayingOffset(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getLoop(const Napi::CallbackInfo& info);
    void setLoop(const Napi::CallbackInfo& info, const Napi::Value& val);
    /* Music */
    Napi::Value openFromFile(const Napi::CallbackInfo& info);
    Napi::Value openFromMemory(const Napi::CallbackInfo& info);
    Napi::Value getDuration(const Napi::CallbackInfo& info);
    Napi::Value getLoopPoints(const Napi::CallbackInfo& info);
    void setLoopPoints(const Napi::CallbackInfo& info, const Napi::Value& val);
};

}
