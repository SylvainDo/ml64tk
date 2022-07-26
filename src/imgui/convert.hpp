#pragma once

#include "core/type/convert.hpp"

#include <imgui.h>

namespace imgui {
inline namespace convert {

inline ImVec2 asVec2(const Napi::Value& val) {
    using namespace core::type::convert;

    const auto obj = valueAsObject(val);
    return { asF32(obj.Get("x")), asF32(obj.Get("y")) };
}

inline ImVec2 asVec2Or(const Napi::Value& val, ImVec2 defaultVal) {
    return val.IsUndefined() ? defaultVal : asVec2(val);
}

inline Napi::Value fromVec2(Napi::Env env, ImVec2 val) {
    using namespace core::type::convert;

    const auto obj = Napi::Object::New(env);
    obj.Set("x", fromF32(env, val.x));
    obj.Set("y", fromF32(env, val.y));

    return obj;
}

inline Napi::Value fromVec2(Napi::Env env, int x, int y) {
    using namespace core::type::convert;

    const auto obj = Napi::Object::New(env);
    obj.Set("x", fromS32(env, x));
    obj.Set("y", fromS32(env, y));

    return obj;
}

inline ImVec4 asVec4(const Napi::Value& val) {
    using namespace core::type::convert;

    const auto obj = valueAsObject(val);
    return { asF32(obj.Get("x")), asF32(obj.Get("y")), asF32(obj.Get("z")), asF32(obj.Get("w")) };
}

inline ImVec4 asVec4Or(const Napi::Value& val, ImVec4 defaultVal) {
    return val.IsUndefined() ? defaultVal : asVec4(val);
}

inline Napi::Value fromVec4(Napi::Env env, ImVec4 val) {
    using namespace core::type::convert;

    const auto obj = Napi::Object::New(env);
    obj.Set("x", fromF32(env, val.x));
    obj.Set("y", fromF32(env, val.y));
    obj.Set("z", fromF32(env, val.z));
    obj.Set("w", fromF32(env, val.w));

    return obj;
}

inline Napi::Value fromVec4(Napi::Env env, int x, int y, int z, int w) {
    using namespace core::type::convert;

    const auto obj = Napi::Object::New(env);
    obj.Set("x", fromS32(env, x));
    obj.Set("y", fromS32(env, y));
    obj.Set("z", fromS32(env, z));
    obj.Set("w", fromS32(env, w));

    return obj;
}

inline ImColor asColor(const Napi::Value& val) {
    using namespace core::type::convert;

    auto obj = valueAsObject(val);
    return { asF32(obj.Get("x")), asF32(obj.Get("y")), asF32(obj.Get("z")), asF32(obj.Get("w")) };
}

inline ImColor asColorOr(const Napi::Value& val, ImColor defaultVal) {
    return val.IsUndefined() ? defaultVal : asColor(val);
}

inline Napi::Value fromColor(Napi::Env env, ImColor val) {
    using namespace core::type::convert;

    auto obj = Napi::Object::New(env);
    obj.Set("x", fromF32(env, val.Value.x));
    obj.Set("y", fromF32(env, val.Value.y));
    obj.Set("z", fromF32(env, val.Value.z));
    obj.Set("w", fromF32(env, val.Value.w));

    return obj;
}

}
}
