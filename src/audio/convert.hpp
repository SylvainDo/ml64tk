#pragma once

#include "core/type/convert.hpp"

#include <SFML/Audio/Music.hpp>

namespace audio {
inline namespace convert {

inline sf::Vector3f asVec3(const Napi::Value& val) {
    using namespace core::type::convert;

    const auto obj = valueAsObject(val);
    return { asF32(obj.Get("x")), asF32(obj.Get("y")), asF32(obj.Get("z")) };
}

inline sf::Vector3f asVec3Or(const Napi::Value& val, const sf::Vector3f& defaultVal) {
    return val.IsUndefined() ? defaultVal : asVec3(val);
}

inline Napi::Value fromVec3(Napi::Env env, const sf::Vector3f& val) {
    using namespace core::type::convert;

    const auto obj = Napi::Object::New(env);
    obj.Set("x", fromF32(env, val.x));
    obj.Set("y", fromF32(env, val.y));
    obj.Set("z", fromF32(env, val.z));

    return obj;
}

inline sf::Time asTime(const Napi::Value& val) {
    using namespace core::type::convert;

    return sf::milliseconds(asS32(val));
}

inline sf::Time asTimeOr(const Napi::Value& val, sf::Time defaultVal) {
    return val.IsUndefined() ? defaultVal : asTime(val);
}

inline Napi::Value fromTime(Napi::Env env, sf::Time val) {
    using namespace core::type::convert;

    return fromS32(env, val.asMilliseconds());
}

inline sf::Music::TimeSpan asTimeSpan(const Napi::Value& val) {
    using namespace core::type::convert;

    const auto obj = valueAsObject(val);
    return sf::Music::TimeSpan{
        sf::milliseconds(asS32(obj.Get("offset"))),
        sf::milliseconds(asS32(obj.Get("length"))) };
}

inline sf::Music::TimeSpan asTimeSpanOr(const Napi::Value& val, const sf::Music::TimeSpan& defaultVal) {
    return val.IsUndefined() ? defaultVal : asTimeSpan(val);
}

inline Napi::Value fromTimeSpan(Napi::Env env, const sf::Music::TimeSpan& val) {
    using namespace core::type::convert;

    const auto obj = Napi::Object::New(env);
    obj.Set("offset", fromS32(env, val.offset.asMilliseconds()));
    obj.Set("length", fromS32(env, val.length.asMilliseconds()));

    return obj;
}

}
}
