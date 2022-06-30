#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

#include <napi.h>

namespace core::type::convert {

inline Napi::Object valueAsObject(const Napi::Value& value) {
    return value.As<Napi::Object>();
}

inline Napi::Array valueAsArray(const Napi::Value& value) {
    return value.As<Napi::Array>();
}

inline Napi::Function valueAsFunction(const Napi::Value& value) {
    return value.As<Napi::Function>();
}

inline Napi::Number valueAsNumber(const Napi::Value& value) {
    return value.As<Napi::Number>();
}

inline Napi::Boolean valueAsBool(const Napi::Value& value) {
    return value.As<Napi::Boolean>();
}

inline Napi::String valueAsString(const Napi::Value& value) {
    return value.As<Napi::String>();
}

inline Napi::BigInt valueAsBigInt(const Napi::Value& value) {
    return value.As<Napi::BigInt>();
}

inline std::uint8_t asU8(const Napi::Value& value) {
    return static_cast<std::uint8_t>(valueAsNumber(value).Uint32Value());
}

inline std::uint8_t asU8Or(const Napi::Value& value, std::uint8_t defaultValue) {
    return value.IsUndefined() ? defaultValue : asU8(value);
}

inline std::uint16_t asU16(const Napi::Value& value) {
    return static_cast<std::uint16_t>(valueAsNumber(value).Uint32Value());
}

inline std::uint16_t asU16Or(const Napi::Value& value, std::uint16_t defaultValue) {
    return value.IsUndefined() ? defaultValue : asU16(value);
}

inline std::uint32_t asU32(const Napi::Value& value) {
    return static_cast<std::uint32_t>(valueAsNumber(value).Uint32Value());
}

inline std::uint32_t asU32Or(const Napi::Value& value, std::uint32_t defaultValue) {
    return value.IsUndefined() ? defaultValue : asU32(value);
}

inline std::uint64_t asU64(const Napi::Value& value) {
    bool lossless;
    const auto v = valueAsBigInt(value).Uint64Value(&lossless);
    if (!lossless) throw Napi::Error::New(value.Env(), "couldn't convert bigint to unsigned long long losslessly");
    return v;
}

inline std::uint64_t asU64Or(const Napi::Value& value, std::uint64_t defaultValue) {
    return value.IsUndefined() ? defaultValue : asU64(value);
}

inline std::int8_t asS8(const Napi::Value& value) {
    return static_cast<std::int8_t>(valueAsNumber(value).Int32Value());
}

inline std::int8_t asS8Or(const Napi::Value& value, std::int8_t defaultValue) {
    return value.IsUndefined() ? defaultValue : asS8(value);
}

inline std::int16_t asS16(const Napi::Value& value) {
    return static_cast<std::int16_t>(valueAsNumber(value).Int32Value());
}

inline std::int16_t asS16Or(const Napi::Value& value, std::int16_t defaultValue) {
    return value.IsUndefined() ? defaultValue : asS16(value);
}

inline std::int32_t asS32(const Napi::Value& value) {
    return static_cast<std::int32_t>(valueAsNumber(value).Int32Value());
}

inline std::int32_t asS32Or(const Napi::Value& value, std::int32_t defaultValue) {
    return value.IsUndefined() ? defaultValue : asS32(value);
}

inline std::int64_t asS64(const Napi::Value& value) {
    bool lossless;
    const auto v = valueAsBigInt(value).Int64Value(&lossless);
    if (!lossless) throw Napi::Error::New(value.Env(), "couldn't convert bigint to long long losslessly");
    return v;
}

inline std::int64_t asS64Or(const Napi::Value& value, std::int64_t defaultValue) {
    return value.IsUndefined() ? defaultValue : asS64(value);
}

inline float asF32(const Napi::Value& value) {
    return valueAsNumber(value).FloatValue();
}

inline float asF32Or(const Napi::Value& value, float defaultValue) {
    return value.IsUndefined() ? defaultValue : asF32(value);
}

inline double asF64(const Napi::Value& value) {
    return valueAsNumber(value).DoubleValue();
}

inline double asF64Or(const Napi::Value& value, double defaultValue) {
    return value.IsUndefined() ? defaultValue : asF64(value);
}

inline bool asBool(const Napi::Value& value) {
    return valueAsBool(value).Value();
}

inline bool asBoolOr(const Napi::Value& value, bool defaultValue) {
    return value.IsUndefined() ? defaultValue : asBool(value);
}

inline bool toBool(const Napi::Value& value) {
    return value.ToBoolean().Value();
}

inline std::string asStrUtf8(const Napi::Value& value) {
    return valueAsString(value).Utf8Value();
}

inline std::string asStrUtf8Or(const Napi::Value& value, const std::string& defaultValue) {
    return value.IsUndefined() ? defaultValue : asStrUtf8(value);
}

inline std::u16string asStrUtf16(const Napi::Value& value) {
    return valueAsString(value).Utf16Value();
}

inline std::u16string asStrUtf16Or(const Napi::Value& value, const std::u16string& defaultValue) {
    return value.IsUndefined() ? defaultValue : asStrUtf16(value);
}

inline std::filesystem::path asPath(const Napi::Value& value) {
#ifdef _WIN32
    return asStrUtf16(value);
#else
    return asStrUtf8(value);
#endif
}

inline std::filesystem::path asPathOr(const Napi::Value& value, const std::filesystem::path& defaultValue) {
    return value.IsUndefined() ? defaultValue : asPath(value);
}

inline std::size_t asSize(const Napi::Value& value) {
    return static_cast<std::size_t>(asU64(value));
}

inline std::size_t asSizeOr(const Napi::Value& value, std::size_t defaultValue) {
    return value.IsUndefined() ? defaultValue : asSize(value);
}

inline Napi::Value fromU8(Napi::Env env, std::uint8_t value) {
    return Napi::Number::New(env, value);
}

inline Napi::Value fromU16(Napi::Env env, std::uint16_t value) {
    return Napi::Number::New(env, value);
}

inline Napi::Value fromU32(Napi::Env env, std::uint32_t value) {
    return Napi::Number::New(env, value);
}

inline Napi::Value fromU64(Napi::Env env, std::uint64_t value) {
    return Napi::BigInt::New(env, value);
}

inline Napi::Value fromS8(Napi::Env env, std::int8_t value) {
    return Napi::Number::New(env, value);
}

inline Napi::Value fromS16(Napi::Env env, std::int16_t value) {
    return Napi::Number::New(env, value);
}

inline Napi::Value fromS32(Napi::Env env, std::int32_t value) {
    return Napi::Number::New(env, value);
}

inline Napi::Value fromS64(Napi::Env env, std::int64_t value) {
    return Napi::BigInt::New(env, value);
}

inline Napi::Value fromF32(Napi::Env env, float value) {
    return Napi::Number::New(env, value);
}

inline Napi::Value fromF64(Napi::Env env, double value) {
    return Napi::Number::New(env, value);
}

inline Napi::Value fromBool(Napi::Env env, bool value) {
    return Napi::Boolean::New(env, value);
}

inline Napi::Value fromStrUtf8(Napi::Env env, const char* value) {
    return Napi::String::New(env, value);
}

inline Napi::Value fromStrUtf8(Napi::Env env, const char* value, std::size_t length) {
    return Napi::String::New(env, value, length);
}

inline Napi::Value fromStrUtf8(Napi::Env env, const std::string& value) {
    return Napi::String::New(env, value);
}

inline Napi::Value fromStrUtf8(Napi::Env env, std::string_view value) {
    return Napi::String::New(env, value.data(), value.size());
}

inline Napi::Value fromStrUtf16(Napi::Env env, const char16_t* value) {
    return Napi::String::New(env, value);
}

inline Napi::Value fromStrUtf16(Napi::Env env, const char16_t* value, std::size_t length) {
    return Napi::String::New(env, value, length);
}

inline Napi::Value fromStrUtf16(Napi::Env env, const std::u16string& value) {
    return Napi::String::New(env, value);
}

inline Napi::Value fromStrUtf16(Napi::Env env, std::u16string_view value) {
    return Napi::String::New(env, value.data(), value.size());
}

inline Napi::Value fromPath(Napi::Env env, const std::filesystem::path& value) {
    const auto str = value.generic_u8string();
    return Napi::String::New(env, reinterpret_cast<const char*>(str.data()), str.size());
}

inline Napi::Value fromSize(Napi::Env env, std::size_t size) {
    return fromU64(env, size);
}

}
