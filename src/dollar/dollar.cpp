#include "audio/convert.hpp"
#include "core/largeInteger.hpp"
#include "dollar/dollar.hpp"
#include "imgui/convert.hpp"

using namespace audio;
using namespace core;
using namespace imgui;

namespace dollar {

Napi::Value xy(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), { asF32(info[0]), asF32(info[1]) });
}

Napi::Value xyz(const Napi::CallbackInfo& info) {
    if (info.Length() == 3) {
        return fromVec3(info.Env(), { asF32(info[0]), asF32(info[1]), asF32(info[2]) });
    }
    else {
        const auto xy = asVec2(info[0]);
        return fromVec3(info.Env(), { xy.x, xy.y, asF32(info[1]) });
    }
}

Napi::Value xyzw(const Napi::CallbackInfo& info) {
    if (info.Length() == 4) {
        return fromVec4(info.Env(), { asF32(info[0]), asF32(info[1]), asF32(info[2]), asF32(info[3]) });
    }
    else {
        if (info[1].IsObject()) {
            const auto xy = asVec2(info[0]);
            const auto zw = asVec2(info[1]);
            return fromVec4(info.Env(), { xy.x, xy.y, zw.x, zw.y });
        }
        else {
            const auto xyz = asVec3(info[0]);
            return fromVec4(info.Env(), { xyz.x, xyz.y, xyz.z, asF32(info[1]) });
        }
    }
}

Napi::Value rgbf(const Napi::CallbackInfo& info) {
    return fromVec4(info.Env(), { asF32(info[0]), asF32(info[1]), asF32(info[2]), 1.0f });
}

Napi::Value rgbaf(const Napi::CallbackInfo& info) {
    return fromVec4(info.Env(), { asF32(info[0]), asF32(info[1]), asF32(info[2]), asF32(info[3]) });
}

Napi::Value rgb(const Napi::CallbackInfo& info) {
    const float m = 1 / 255.0f;
    return fromVec4(info.Env(), { asF32(info[0]) * m, asF32(info[1]) * m, asF32(info[2]) * m, 1.0f });
}

Napi::Value rgba(const Napi::CallbackInfo& info) {
    const float m = 1 / 255.0f;
    return fromVec4(info.Env(), { asF32(info[0]) * m, asF32(info[1]) * m, asF32(info[2]) * m, asF32(info[3]) * m });
}

Napi::Value v2eq(const Napi::CallbackInfo& info) {
    const auto lhs = asVec2(info[0]);
    const auto rhs = asVec2(info[1]);
    return fromBool(info.Env(), lhs.x == rhs.x && lhs.y == rhs.y);
}

Napi::Value v2add(const Napi::CallbackInfo& info) {
    const auto lhs = asVec2(info[0]);
    const auto rhs = asVec2(info[1]);
    return fromVec2(info.Env(), { lhs.x + rhs.x, lhs.y + rhs.y });
}

Napi::Value v2mul(const Napi::CallbackInfo& info) {
    const auto lhs = asVec2(info[0]);
    const auto rhs = asF32(info[1]);
    return fromVec2(info.Env(), { lhs.x * rhs, lhs.y * rhs });
}

Napi::Value v3eq(const Napi::CallbackInfo& info) {
    const auto lhs = asVec3(info[0]);
    const auto rhs = asVec3(info[1]);
    return fromBool(info.Env(), lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
}

Napi::Value v4eq(const Napi::CallbackInfo& info) {
    const auto lhs = asVec4(info[0]);
    const auto rhs = asVec4(info[1]);
    return fromBool(info.Env(), lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w);
}

inline float lerp_(float v0, float v1, float t) {
    return (1 - t) * v0 + t * v1;
}

Napi::Value lerp(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), lerp_(asF32(info[0]), asF32(info[1]), asF32(info[2])));
}

Napi::Value v2lerp(const Napi::CallbackInfo& info) {
    const auto from = asVec2(info[0]);
    const auto to = asVec2(info[1]);
    const auto t = asF32(info[2]);
    return fromVec2(info.Env(), {
        lerp_(from.x, to.x, t),
        lerp_(from.y, to.y, t)
    });
}

Napi::Value v3lerp(const Napi::CallbackInfo& info) {
    const auto from = asVec3(info[0]);
    const auto to = asVec3(info[1]);
    const auto t = asF32(info[2]);
    return fromVec3(info.Env(), {
        lerp_(from.x, to.x, t),
        lerp_(from.y, to.y, t),
        lerp_(from.z, to.z, t)
    });
}

Napi::Value v4lerp(const Napi::CallbackInfo& info) {
    const auto from = asVec4(info[0]);
    const auto to = asVec4(info[1]);
    const auto t = asF32(info[2]);
    return fromVec4(info.Env(), {
        lerp_(from.x, to.x, t),
        lerp_(from.y, to.y, t),
        lerp_(from.z, to.z, t),
        lerp_(from.w, to.w, t)
    });
}

Napi::Value sli(const Napi::CallbackInfo& info) {
    return SignedLargeInteger::create(info.Env(), SignedLargeInteger::get(info[0]));
}

Napi::Value slinum(const Napi::CallbackInfo& info) {
    return fromF64(info.Env(), static_cast<double>(SignedLargeInteger::get(info[0])));
}

Napi::Value slibig(const Napi::CallbackInfo& info) {
    return fromS64(info.Env(), SignedLargeInteger::get(info[0]));
}

Napi::Value uli(const Napi::CallbackInfo& info) {
    return UnsignedLargeInteger::create(info.Env(), UnsignedLargeInteger::get(info[0]));
}

Napi::Value ulinum(const Napi::CallbackInfo& info) {
    return fromF64(info.Env(), static_cast<double>(UnsignedLargeInteger::get(info[0])));
}

Napi::Value ulibig(const Napi::CallbackInfo& info) {
    return fromU64(info.Env(), UnsignedLargeInteger::get(info[0]));
}

Napi::Object initialize(Napi::Env env, Napi::Object exports) {
    exports.Set("xy", Napi::Function::New(env, xy));
    exports.Set("xyz", Napi::Function::New(env, xyz));
    exports.Set("xyzw", Napi::Function::New(env, xyzw));
    exports.Set("rgbf", Napi::Function::New(env, rgbf));
    exports.Set("rgbaf", Napi::Function::New(env, rgbaf));
    exports.Set("rgb", Napi::Function::New(env, rgb));
    exports.Set("rgba", Napi::Function::New(env, rgba));
    exports.Set("v2eq", Napi::Function::New(env, v2eq));
    exports.Set("v2add", Napi::Function::New(env, v2add));
    exports.Set("v2mul", Napi::Function::New(env, v2mul));
    exports.Set("v3eq", Napi::Function::New(env, v3eq));
    exports.Set("v4eq", Napi::Function::New(env, v4eq));
    exports.Set("lerp", Napi::Function::New(env, lerp));
    exports.Set("v2lerp", Napi::Function::New(env, v2lerp));
    exports.Set("v3lerp", Napi::Function::New(env, v3lerp));
    exports.Set("v4lerp", Napi::Function::New(env, v4lerp));
    exports.Set("sli", Napi::Function::New(env, sli));
    exports.Set("slinum", Napi::Function::New(env, slinum));
    exports.Set("slibig", Napi::Function::New(env, slibig));
    exports.Set("uli", Napi::Function::New(env, uli));
    exports.Set("ulinum", Napi::Function::New(env, ulinum));
    exports.Set("ulibig", Napi::Function::New(env, ulibig));

    return exports;
}

}
