#pragma once

#include "core/convert.hpp"

#include <SFML/Audio/Music.hpp>

namespace audio {
inline namespace convert {

inline sf::Vector3f asVec3(const Napi::Value& val) {
    const auto obj = core::valueAsObject(val);
    return { core::asF32(obj.Get("x")), core::asF32(obj.Get("y")), core::asF32(obj.Get("z")) };
}

inline sf::Vector3f asVec3Or(const Napi::Value& val, const sf::Vector3f& defaultVal) {
    return val.IsUndefined() ? defaultVal : asVec3(val);
}

inline Napi::Value fromVec3(Napi::Env env, const sf::Vector3f& val) {
    const auto obj = Napi::Object::New(env);
    obj.Set("x", core::fromF32(env, val.x));
    obj.Set("y", core::fromF32(env, val.y));
    obj.Set("z", core::fromF32(env, val.z));

    return obj;
}

inline sf::Time asTime(const Napi::Value& val) {
    return sf::milliseconds(core::asS32(val));
}

inline sf::Time asTimeOr(const Napi::Value& val, sf::Time defaultVal) {
    return val.IsUndefined() ? defaultVal : asTime(val);
}

inline Napi::Value fromTime(Napi::Env env, sf::Time val) {
    return core::fromS32(env, val.asMilliseconds());
}

inline sf::Music::TimeSpan asTimeSpan(const Napi::Value& val) {
    const auto obj = core::valueAsObject(val);
    return sf::Music::TimeSpan{
        sf::milliseconds(core::asS32(obj.Get("offset"))),
        sf::milliseconds(core::asS32(obj.Get("length"))) };
}

inline sf::Music::TimeSpan asTimeSpanOr(const Napi::Value& val, const sf::Music::TimeSpan& defaultVal) {
    return val.IsUndefined() ? defaultVal : asTimeSpan(val);
}

inline Napi::Value fromTimeSpan(Napi::Env env, const sf::Music::TimeSpan& val) {
    const auto obj = Napi::Object::New(env);
    obj.Set("offset", core::fromS32(env, val.offset.asMilliseconds()));
    obj.Set("length", core::fromS32(env, val.length.asMilliseconds()));

    return obj;
}

}
}
