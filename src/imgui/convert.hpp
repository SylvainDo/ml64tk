#pragma once

#include "core/convert.hpp"

#include <imgui.h>

namespace imgui {
inline namespace convert {

inline ImVec2 asVec2(const Napi::Value& val) {
    const auto obj = core::valueAsObject(val);
    return { core::asF32(obj.Get("x")), core::asF32(obj.Get("y")) };
}

inline ImVec2 asVec2Or(const Napi::Value& val, ImVec2 defaultVal) {
    return val.IsUndefined() ? defaultVal : asVec2(val);
}

inline Napi::Value fromVec2(Napi::Env env, ImVec2 val) {
    const auto obj = Napi::Object::New(env);
    obj.Set("x", core::fromF32(env, val.x));
    obj.Set("y", core::fromF32(env, val.y));

    return obj;
}

inline Napi::Value fromVec2(Napi::Env env, int x, int y) {
    const auto obj = Napi::Object::New(env);
    obj.Set("x", core::fromS32(env, x));
    obj.Set("y", core::fromS32(env, y));

    return obj;
}

inline ImVec4 asVec4(const Napi::Value& val) {
    const auto obj = core::valueAsObject(val);
    return { core::asF32(obj.Get("x")), core::asF32(obj.Get("y")), core::asF32(obj.Get("z")), core::asF32(obj.Get("w")) };
}

inline ImVec4 asVec4Or(const Napi::Value& val, const ImVec4& defaultVal) {
    return val.IsUndefined() ? defaultVal : asVec4(val);
}

inline Napi::Value fromVec4(Napi::Env env, const ImVec4& val) {
    const auto obj = Napi::Object::New(env);
    obj.Set("x", core::fromF32(env, val.x));
    obj.Set("y", core::fromF32(env, val.y));
    obj.Set("z", core::fromF32(env, val.z));
    obj.Set("w", core::fromF32(env, val.w));

    return obj;
}

inline Napi::Value fromVec4(Napi::Env env, int x, int y, int z, int w) {
    const auto obj = Napi::Object::New(env);
    obj.Set("x", core::fromS32(env, x));
    obj.Set("y", core::fromS32(env, y));
    obj.Set("z", core::fromS32(env, z));
    obj.Set("w", core::fromS32(env, w));

    return obj;
}

inline ImColor asColor(const Napi::Value& val) {
    auto obj = core::valueAsObject(val);
    return { core::asF32(obj.Get("x")), core::asF32(obj.Get("y")), core::asF32(obj.Get("z")), core::asF32(obj.Get("w")) };
}

inline ImColor asColorOr(const Napi::Value& val, const ImColor& defaultVal) {
    return val.IsUndefined() ? defaultVal : asColor(val);
}

inline Napi::Value fromColor(Napi::Env env, const ImColor& val) {
    auto obj = Napi::Object::New(env);
    obj.Set("x", core::fromF32(env, val.Value.x));
    obj.Set("y", core::fromF32(env, val.Value.y));
    obj.Set("z", core::fromF32(env, val.Value.z));
    obj.Set("w", core::fromF32(env, val.Value.w));

    return obj;
}

}
}
