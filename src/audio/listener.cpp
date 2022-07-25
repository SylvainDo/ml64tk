#include "audio/convert.hpp"
#include "audio/listener.hpp"

#include <fmt/format.h>
#include <SFML/Audio/Listener.hpp>

using namespace core::type::convert;

namespace audio {

Napi::FunctionReference Listener::m_ctor;

Napi::Object Listener::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "Listener";
    const auto class_ = DefineClass(env, className, {
        StaticAccessor<&Listener::toDebugString>("debugString"),

        StaticAccessor<&Listener::getGlobalVolume, &Listener::setGlobalVolume>("globalVolume"),
        StaticAccessor<&Listener::getPosition, &Listener::setPosition>("position"),
        StaticAccessor<&Listener::getDirection, &Listener::setDirection>("direction"),
        StaticAccessor<&Listener::getUpVector, &Listener::setUpVector>("upVector")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Listener::Listener(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Listener>{ info } {}

inline std::string formatVec3(const sf::Vector3f& v) {
    return fmt::format("({},{},{})", v.x, v.y, v.z);
}

Napi::Value Listener::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("Listener (globalVolume={}; position={}; direction={}; upVector={})",
        sf::Listener::getGlobalVolume(), formatVec3(sf::Listener::getPosition()),
        formatVec3(sf::Listener::getDirection()), formatVec3(sf::Listener::getUpVector())));
}

Napi::Value Listener::getGlobalVolume(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), sf::Listener::getGlobalVolume());
}

void Listener::setGlobalVolume(const Napi::CallbackInfo&, const Napi::Value& val) {
    sf::Listener::setGlobalVolume(asF32(val));
}

Napi::Value Listener::getPosition(const Napi::CallbackInfo& info) {
    return fromVec3(info.Env(), sf::Listener::getPosition());
}

void Listener::setPosition(const Napi::CallbackInfo&, const Napi::Value& val) {
    sf::Listener::setPosition(asVec3(val));
}

Napi::Value Listener::getDirection(const Napi::CallbackInfo& info) {
    return fromVec3(info.Env(), sf::Listener::getDirection());
}

void Listener::setDirection(const Napi::CallbackInfo&, const Napi::Value& val) {
    sf::Listener::setDirection(asVec3(val));
}

Napi::Value Listener::getUpVector(const Napi::CallbackInfo& info) {
    return fromVec3(info.Env(), sf::Listener::getUpVector());
}

void Listener::setUpVector(const Napi::CallbackInfo&, const Napi::Value& val) {
    sf::Listener::setUpVector(asVec3(val));
}

}
