#include "audio/convert.hpp"
#include "audio/music.hpp"
#include "core/type/instanceOf.hpp"

#include <fmt/format.h>

using namespace core::type::convert;

namespace audio {

Napi::FunctionReference Music::m_ctor;

Napi::Object Music::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "Music";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&Music::getTypeId>("typeId"),
        InstanceAccessor<&Music::toDebugString>("debugString"),
        InstanceMethod<&Music::ref>("ref"),
        InstanceMethod<&Music::unref>("unref"),

        InstanceMethod<&Music::equals>("equals"),
        /* SoundSource */
        InstanceAccessor<&Music::getPitch, &Music::setPitch>("pitch"),
        InstanceAccessor<&Music::getVolume, &Music::setVolume>("volume"),
        InstanceAccessor<&Music::getPosition, &Music::setPosition>("position"),
        InstanceAccessor<&Music::getRelativeToListener, &Music::setRelativeToListener>("relativeToListener"),
        InstanceAccessor<&Music::getMinDistance, &Music::setMinDistance>("minDistance"),
        InstanceAccessor<&Music::getAttenuation, &Music::setAttenuation>("attenuation"),
        InstanceMethod<&Music::play>("play"),
        InstanceMethod<&Music::pause>("pause"),
        InstanceMethod<&Music::stop>("stop"),
        InstanceAccessor<&Music::getStatus>("status"),
        /* SoundStream */
        InstanceAccessor<&Music::getChannelCount>("channelCount"),
        InstanceAccessor<&Music::getSampleRate>("sampleRate"),
        InstanceAccessor<&Music::getPlayingOffset, &Music::setPlayingOffset>("playingOffset"),
        InstanceAccessor<&Music::getLoop, &Music::setLoop>("loop"),
        /* Music */
        InstanceMethod<&Music::openFromFile>("openFromFile"),
        InstanceMethod<&Music::openFromMemory>("openFromMemory"),
        InstanceAccessor<&Music::getDuration>("duration"),
        InstanceAccessor<&Music::getLoopPoints, &Music::setLoopPoints>("loopPoints")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Music::Music(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Music>{ info } {}

Napi::Value Music::getTypeId(const Napi::CallbackInfo& info) {
    return core::type::fromTypeId<Music>(info.Env());
}

Napi::Value Music::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("Music (instance={}; pitch={}; volume={}; position=({},{},{}); relativeToListener={}; minDistance={}; attenuation={}; status={}; channelCount={}; sampleRate={} Hz; playingOffset={} ms; loop={}; duration={} ms; loopPoints=(offset={} ms; length={} ms))",
        fmt::ptr(&m_inst), m_inst.getPitch(), m_inst.getVolume(), m_inst.getPosition().x, m_inst.getPosition().y, m_inst.getPosition().z,
        m_inst.isRelativeToListener(), m_inst.getMinDistance(), m_inst.getAttenuation(), m_inst.getStatus(), m_inst.getChannelCount(),
        m_inst.getSampleRate(), m_inst.getPlayingOffset().asMilliseconds(), m_inst.getLoop(), m_inst.getDuration().asMilliseconds(),
        m_inst.getLoopPoints().offset.asMilliseconds(), m_inst.getLoopPoints().length.asMilliseconds()));
}

Napi::Value Music::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value Music::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value Music::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), this == Music::unwrap(info[0]));
}

Napi::Value Music::getPitch(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_inst.getPitch());
}

void Music::setPitch(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setPitch(asF32(val));
}

Napi::Value Music::getVolume(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_inst.getVolume());
}

void Music::setVolume(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setVolume(asF32(val));
}

Napi::Value Music::getPosition(const Napi::CallbackInfo& info) {
    return fromVec3(info.Env(), m_inst.getPosition());
}

void Music::setPosition(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setPosition(asVec3(val));
}

Napi::Value Music::getRelativeToListener(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_inst.isRelativeToListener());
}

void Music::setRelativeToListener(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setRelativeToListener(asBool(val));
}

Napi::Value Music::getMinDistance(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_inst.getMinDistance());
}

void Music::setMinDistance(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setMinDistance(asF32(val));
}

Napi::Value Music::getAttenuation(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_inst.getAttenuation());
}

void Music::setAttenuation(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setAttenuation(asF32(val));
}

Napi::Value Music::play(const Napi::CallbackInfo& info) {
    m_inst.play();
    return info.Env().Undefined();
}

Napi::Value Music::pause(const Napi::CallbackInfo& info) {
    m_inst.pause();
    return info.Env().Undefined();
}

Napi::Value Music::stop(const Napi::CallbackInfo& info) {
    m_inst.stop();
    return info.Env().Undefined();
}

Napi::Value Music::getStatus(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_inst.getStatus());
}

Napi::Value Music::getChannelCount(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_inst.getChannelCount());
}

Napi::Value Music::getSampleRate(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_inst.getSampleRate());
}

Napi::Value Music::getPlayingOffset(const Napi::CallbackInfo& info) {
    return fromTime(info.Env(), m_inst.getPlayingOffset());
}

void Music::setPlayingOffset(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setPlayingOffset(asTime(val));
}

Napi::Value Music::getLoop(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_inst.getLoop());
}

void Music::setLoop(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setLoop(asBool(val));
}

Napi::Value Music::openFromFile(const Napi::CallbackInfo& info) {
    const auto filename = asStrUtf8(info[0]);
    bool ok = m_inst.openFromFile(filename);
    if (!ok) throw Napi::Error::New(info.Env(), fmt::format("failed to open music from file `{}`", filename));
    return info.Env().Undefined();
}

Napi::Value Music::openFromMemory(const Napi::CallbackInfo& info) {
    auto data = info[0].As<Napi::Uint8Array>();
    bool ok = m_inst.openFromMemory(data.Data(), data.ByteLength());
    if (!ok) throw Napi::Error::New(info.Env(), "failed to open music from memory");
    m_buf = Napi::Persistent(valueAsObject(info[0]));
    return info.Env().Undefined();
}

Napi::Value Music::getDuration(const Napi::CallbackInfo& info) {
    return fromTime(info.Env(), m_inst.getDuration());
}

Napi::Value Music::getLoopPoints(const Napi::CallbackInfo& info) {
    return fromTimeSpan(info.Env(), m_inst.getLoopPoints());
}

void Music::setLoopPoints(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_inst.setLoopPoints(asTimeSpan(val));
}

}
