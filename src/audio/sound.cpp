#include "audio/convert.hpp"
#include "audio/sound.hpp"
#include "audio/soundBuffer.hpp"
#include "core/type/instanceOf.hpp"

#include <fmt/format.h>

using namespace core::type::convert;

namespace audio {

Napi::FunctionReference Sound::m_ctor;

Napi::Object Sound::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "Sound";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&Sound::getTypeId>("typeId"),
        InstanceAccessor<&Sound::toDebugString>("debugString"),
        InstanceMethod<&Sound::ref>("ref"),
        InstanceMethod<&Sound::unref>("unref"),

        InstanceMethod<&Sound::equals>("equals"),
        /* SoundSource */
        InstanceAccessor<&Sound::getPitch, &Sound::setPitch>("pitch"),
        InstanceAccessor<&Sound::getVolume, &Sound::setVolume>("volume"),
        InstanceAccessor<&Sound::getPosition, &Sound::setPosition>("position"),
        InstanceAccessor<&Sound::getRelativeToListener, &Sound::setRelativeToListener>("relativeToListener"),
        InstanceAccessor<&Sound::getMinDistance, &Sound::setMinDistance>("minDistance"),
        InstanceAccessor<&Sound::getAttenuation, &Sound::setAttenuation>("attenuation"),
        InstanceMethod<&Sound::play>("play"),
        InstanceMethod<&Sound::pause>("pause"),
        InstanceMethod<&Sound::stop>("stop"),
        InstanceAccessor<&Sound::getStatus>("status"),
        /* Sound */
        InstanceAccessor<&Sound::getBuffer, &Sound::setBuffer>("buffer"),
        InstanceAccessor<&Sound::getLoop, &Sound::setLoop>("loop"),
        InstanceAccessor<&Sound::getPlayingOffset, &Sound::setPlayingOffset>("playingOffset")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Sound::Sound(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Sound>{ info } {}

Napi::Value Sound::getTypeId(const Napi::CallbackInfo& info) {
    return core::type::fromTypeId<Sound>(info.Env());
}

Napi::Value Sound::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("Sound (instance={}; pitch={}; volume={}; position=({},{},{}); relativeToListener={}; minDistance={}; attenuation={}; status={}; buffer={}; loop={}; playingOffset={})",
        fmt::ptr(&m_inst), m_inst.getPitch(), m_inst.getVolume(), m_inst.getPosition().x, m_inst.getPosition().y, m_inst.getPosition().z,
        m_inst.isRelativeToListener(), m_inst.getMinDistance(), m_inst.getAttenuation(), m_inst.getStatus(),
        m_buf.IsEmpty() ? "undefined" : fmt::format("{}", fmt::ptr(SoundBuffer::get(m_buf.Value()))), m_inst.getLoop(),
        m_inst.getPlayingOffset().asMilliseconds()));
}

Napi::Value Sound::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value Sound::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value Sound::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), this == Sound::unwrap(info[0]));
}

Napi::Value Sound::getPitch(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_inst.getPitch());
}

void Sound::setPitch(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setPitch(asF32(val));
}

Napi::Value Sound::getVolume(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_inst.getVolume());
}

void Sound::setVolume(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setVolume(asF32(val));
}

Napi::Value Sound::getPosition(const Napi::CallbackInfo& info) {
    return fromVec3(info.Env(), m_inst.getPosition());
}

void Sound::setPosition(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setPosition(asVec3(val));
}

Napi::Value Sound::getRelativeToListener(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_inst.isRelativeToListener());
}

void Sound::setRelativeToListener(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setRelativeToListener(asBool(val));
}

Napi::Value Sound::getMinDistance(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_inst.getMinDistance());
}

void Sound::setMinDistance(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setMinDistance(asF32(val));
}

Napi::Value Sound::getAttenuation(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_inst.getAttenuation());
}

void Sound::setAttenuation(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setAttenuation(asF32(val));
}

Napi::Value Sound::play(const Napi::CallbackInfo& info) {
    m_inst.play();
    return info.Env().Undefined();
}

Napi::Value Sound::pause(const Napi::CallbackInfo& info) {
    m_inst.pause();
    return info.Env().Undefined();
}

Napi::Value Sound::stop(const Napi::CallbackInfo& info) {
    m_inst.stop();
    return info.Env().Undefined();
}

Napi::Value Sound::getStatus(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_inst.getStatus());
}

Napi::Value Sound::getBuffer(const Napi::CallbackInfo&) {
    return m_buf.Value();
}

void Sound::setBuffer(const Napi::CallbackInfo&, const Napi::Value& val) {
    if (val.IsUndefined()) {
        m_inst.resetBuffer();
        m_buf.Reset();
    }
    else {
        m_inst.setBuffer(*SoundBuffer::get(val));
        m_buf = Napi::Persistent(valueAsObject(val));
    }
}

Napi::Value Sound::getLoop(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_inst.getLoop());
}

void Sound::setLoop(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setLoop(asBool(val));
}

Napi::Value Sound::getPlayingOffset(const Napi::CallbackInfo& info) {
    return fromTime(info.Env(), m_inst.getPlayingOffset());
}

void Sound::setPlayingOffset(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setPlayingOffset(asTime(val));
}

}
