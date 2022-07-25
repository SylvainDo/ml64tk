#pragma once

#include <napi.h>

namespace audio {

class Listener final : public Napi::ObjectWrap<Listener> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);

    Listener(const Napi::CallbackInfo& info);
    virtual ~Listener() = default;

private:
    static Napi::FunctionReference m_ctor;

    static Napi::Value toDebugString(const Napi::CallbackInfo& info);

    static Napi::Value getGlobalVolume(const Napi::CallbackInfo& info);
    static void setGlobalVolume(const Napi::CallbackInfo& info, const Napi::Value& val);
    static Napi::Value getPosition(const Napi::CallbackInfo& info);
    static void setPosition(const Napi::CallbackInfo& info, const Napi::Value& val);
    static Napi::Value getDirection(const Napi::CallbackInfo& info);
    static void setDirection(const Napi::CallbackInfo& info, const Napi::Value& val);
    static Napi::Value getUpVector(const Napi::CallbackInfo& info);
    static void setUpVector(const Napi::CallbackInfo& info, const Napi::Value& val);
};

}
