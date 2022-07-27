#include "audio/convert.hpp"
#include "audio/soundBuffer.hpp"
#include "core/instanceOf.hpp"

#include <fmt/format.h>

using namespace core;

namespace audio {

Napi::FunctionReference SoundBuffer::m_ctor;

Napi::Object SoundBuffer::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "SoundBuffer";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&SoundBuffer::getTypeId>("typeId"),
        InstanceAccessor<&SoundBuffer::toDebugString>("debugString"),
        InstanceMethod<&SoundBuffer::ref>("ref"),
        InstanceMethod<&SoundBuffer::unref>("unref"),

        InstanceMethod<&SoundBuffer::equals>("equals"),
        InstanceMethod<&SoundBuffer::loadFromFile>("loadFromFile"),
        InstanceMethod<&SoundBuffer::loadFromMemory>("loadFromMemory"),
        InstanceMethod<&SoundBuffer::loadFromSamples>("loadFromSamples"),
        InstanceMethod<&SoundBuffer::saveToFile>("saveToFile"),
        InstanceAccessor<&SoundBuffer::getSamples>("samples"),
        InstanceAccessor<&SoundBuffer::getSampleCount>("sampleCount"),
        InstanceAccessor<&SoundBuffer::getSampleRate>("sampleRate"),
        InstanceAccessor<&SoundBuffer::getChannelCount>("channelCount"),
        InstanceAccessor<&SoundBuffer::getDuration>("duration")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

SoundBuffer::SoundBuffer(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SoundBuffer>{ info } {}

Napi::Value SoundBuffer::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<SoundBuffer>(info.Env());
}

Napi::Value SoundBuffer::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("SoundBuffer (instance={}; sampleCount={}; sampleRate={} Hz; channelCount={}; duration={} ms)",
        fmt::ptr(&m_inst), m_inst.getSampleCount(), m_inst.getSampleRate(), m_inst.getChannelCount(), m_inst.getDuration().asMilliseconds()));
}

Napi::Value SoundBuffer::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value SoundBuffer::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value SoundBuffer::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), this == SoundBuffer::unwrap(info[0]));
}

Napi::Value SoundBuffer::loadFromFile(const Napi::CallbackInfo& info) {
    const auto filename = asStrUtf8(info[0]);
    bool ok = m_inst.loadFromFile(filename);
    if (!ok) throw Napi::Error::New(info.Env(), fmt::format("failed to load sound buffer from file `{}`", filename));
    return info.Env().Undefined();
}

Napi::Value SoundBuffer::loadFromMemory(const Napi::CallbackInfo& info) {
    auto data = info[0].As<Napi::Uint8Array>();
    bool ok = m_inst.loadFromMemory(data.Data(), data.ByteLength());
    if (!ok) throw Napi::Error::New(info.Env(), "failed to load sound buffer from memory");
    return info.Env().Undefined();
}

Napi::Value SoundBuffer::loadFromSamples(const Napi::CallbackInfo& info) {
    auto samples = info[0].As<Napi::Int16Array>();
    bool ok = m_inst.loadFromSamples(samples.Data(), samples.ElementLength(),
        /* channelCount */ asU32(info[1]),
        /* sampleRate */ asU32(info[2]));
    if (!ok) throw Napi::Error::New(info.Env(), "failed to load sound buffer from samples");
    return info.Env().Undefined();
}

Napi::Value SoundBuffer::saveToFile(const Napi::CallbackInfo& info) {
    bool ok = m_inst.saveToFile(/* filename */ asStrUtf8(info[0]));
    if (!ok) throw Napi::Error::New(info.Env(), "failed to save sound buffer to file");
    return info.Env().Undefined();
}

Napi::Value SoundBuffer::getSamples(const Napi::CallbackInfo& info) {
    return Napi::Buffer<std::int16_t>::New(info.Env(), const_cast<std::int16_t*>(m_inst.getSamples()), m_inst.getSampleCount());
}

Napi::Value SoundBuffer::getSampleCount(const Napi::CallbackInfo& info) {
    return fromU64(info.Env(), m_inst.getSampleCount());
}

Napi::Value SoundBuffer::getSampleRate(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_inst.getSampleRate());
}

Napi::Value SoundBuffer::getChannelCount(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_inst.getChannelCount());
}

Napi::Value SoundBuffer::getDuration(const Napi::CallbackInfo& info) {
    return fromTime(info.Env(), m_inst.getDuration());
}

}
