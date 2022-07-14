#pragma once

#include "core/type/convert.hpp"
#include "core/type/instanceOf.hpp"
#include "core/byteswap.hpp"

#include <bit>
#include <bitset>
#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <type_traits>

namespace core {

template <bool Signed>
class LargeInteger;

using SignedLargeInteger = LargeInteger<true>;
using UnsignedLargeInteger = LargeInteger<false>;

template <bool Signed>
struct LargeInteger : Napi::ObjectWrap<LargeInteger<Signed>> {
    using ValueHighPartType = std::conditional_t<Signed, std::int32_t, std::uint32_t>;
    using ValueQuadPartType = std::conditional_t<Signed, std::int64_t, std::uint64_t>;

    union ValueType {
        struct {
            std::uint32_t lowPart;
            ValueHighPartType highPart;
        } u;
        ValueQuadPartType quadPart;
        std::uint8_t bytes[8];
    } value{};

    static inline Napi::FunctionReference constructor;

    static Napi::Object initialize(Napi::Env env, Napi::Object exports) {
        using ObjectWrap_t = Napi::ObjectWrap<LargeInteger<Signed>>;

#ifdef _WIN32
#define InstanceAccessor_ ObjectWrap_t::InstanceAccessor
#define InstanceMethod_ ObjectWrap_t::InstanceMethod
#else
#define InstanceAccessor_ InstanceAccessor
#define InstanceMethod_ InstanceMethod
#endif

        const char* className;
        if constexpr (Signed) className = "SignedLargeInteger"; else className = "UnsignedLargeInteger";

        const auto class_ = ObjectWrap_t::DefineClass(env, className, {
            InstanceAccessor_<&LargeInteger::getTypeId>("typeId"),
            InstanceAccessor_<&LargeInteger::toDebugString>("debugString"),

            InstanceMethod_<&LargeInteger::toNumber>("toNumber"),
            InstanceMethod_<&LargeInteger::toString>("toString"),
            InstanceMethod_<&LargeInteger::toBigInt>("toBigInt"),

            InstanceAccessor_<&LargeInteger::getLowPart, &LargeInteger::setLowPart>("lowPart"),
            InstanceAccessor_<&LargeInteger::getHighPart, &LargeInteger::setHighPart>("highPart"),
            InstanceAccessor_<&LargeInteger::getBytes>("bytes"),

            InstanceMethod_<&LargeInteger::neg>("neg"),
            InstanceMethod_<&LargeInteger::add>("add"),
            InstanceMethod_<&LargeInteger::sub>("sub"),
            InstanceMethod_<&LargeInteger::mul>("mul"),
            InstanceMethod_<&LargeInteger::div>("div"),
            InstanceMethod_<&LargeInteger::mod>("mod"),
            InstanceMethod_<&LargeInteger::band>("band"),
            InstanceMethod_<&LargeInteger::bor>("bor"),
            InstanceMethod_<&LargeInteger::xor_>("xor"),
            InstanceMethod_<&LargeInteger::not_>("not"),
            InstanceMethod_<&LargeInteger::shl>("shl"),
            InstanceMethod_<&LargeInteger::shr>("shr"),
            InstanceMethod_<&LargeInteger::eq>("eq"),
            InstanceMethod_<&LargeInteger::neq>("neq"),
            InstanceMethod_<&LargeInteger::lt>("lt"),
            InstanceMethod_<&LargeInteger::lte>("lte"),
            InstanceMethod_<&LargeInteger::gt>("gt"),
            InstanceMethod_<&LargeInteger::gte>("gte"),
            InstanceMethod_<&LargeInteger::bswap>("bswap"),
            InstanceMethod_<&LargeInteger::bceil>("bceil"),
            InstanceMethod_<&LargeInteger::bfloor>("bfloor"),
            InstanceMethod_<&LargeInteger::bwidth>("bwidth"),
            InstanceMethod_<&LargeInteger::rotl>("rotl"),
            InstanceMethod_<&LargeInteger::rotr>("rotr"),
            InstanceMethod_<&LargeInteger::countlZero>("countlZero"),
            InstanceMethod_<&LargeInteger::countlOne>("countlOne"),
            InstanceMethod_<&LargeInteger::countrZero>("countrZero"),
            InstanceMethod_<&LargeInteger::countrOne>("countrOne"),
            InstanceMethod_<&LargeInteger::popCount>("popCount")
        });

#undef InstanceAccessor_
#undef InstanceMethod_

        constructor = Napi::Persistent(class_);
        constructor.SuppressDestruct();

        exports.Set(className, class_);
        return exports;
    }

    static Napi::Object create(Napi::Env, ValueType val) {
        const auto obj = constructor.New({});
        unwrap(obj)->value = val;
        return obj;
    }

    static LargeInteger* unwrap(const Napi::Object& obj) {
        return LargeInteger::Unwrap(obj);
    }

    static LargeInteger* unwrap(const Napi::Value& val) {
        using namespace core::type::convert;
        return LargeInteger::Unwrap(valueAsObject(val));
    }

    LargeInteger(const Napi::CallbackInfo& info) : Napi::ObjectWrap<LargeInteger>{ info } {
        if (info.Length() == 1) value = valueFrom(info[0]);
    }

    Napi::Value getTypeId(const Napi::CallbackInfo& info) {
        return core::type::fromTypeId<LargeInteger>(info.Env());
    }

    Napi::Value toDebugString(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;

        std::ostringstream oss;
        oss << std::dec << value.quadPart << std::hex << " (QuadPart: 0x" << value.quadPart
            << " LowPart: 0x" << value.u.lowPart << " HighPart: 0x" << value.u.highPart << ")";

        return fromStrUtf8(info.Env(), oss.str());
    }

    Napi::Value toNumber(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromF64(info.Env(), static_cast<double>(value.quadPart));
    }

    Napi::Value toString(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;

        if (info.Length() == 0) {
            return fromStrUtf8(info.Env(), std::to_string(value.quadPart));
        }
        else if (info.Length() == 1) {
            switch (/* base */ asS32(info[0])) {
            case 2: {
                const auto v = static_cast<std::uint64_t>(value.quadPart);
                return fromStrUtf8(info.Env(), std::bitset<64>{ v }.to_string().substr(std::countl_zero(v)));
            }
            case 8: {
                char buf[23];
                std::snprintf(buf, sizeof(buf), "%" PRIo64, value.quadPart);
                return fromStrUtf8(info.Env(), buf);
            }
            case 10:
                return fromStrUtf8(info.Env(), std::to_string(value.quadPart));
            case 16: {
                char buf[17];
                std::snprintf(buf, sizeof(buf), "%" PRIx64, value.quadPart);
                return fromStrUtf8(info.Env(), buf);
            }
            default:
                throw Napi::Error::New(info.Env(), "invalid base");
            }
        }

        return info.Env().Undefined();
    }

    Napi::Value toBigInt(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        if constexpr (Signed) return fromS64(info.Env(), value.u.highPart);
        else return fromU64(info.Env(), value.u.highPart);
    }

    Napi::Value getLowPart(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromU32(info.Env(), value.u.lowPart);
    }

    void setLowPart(const Napi::CallbackInfo&, const Napi::Value& value) {
        using namespace core::type::convert;
        this->value.u.lowPart = asU32(value);
    }

    Napi::Value getHighPart(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        if constexpr (Signed) return fromS32(info.Env(), value.u.highPart);
        else return fromU32(info.Env(), value.u.highPart);
    }

    void setHighPart(const Napi::CallbackInfo&, const Napi::Value& value) {
        using namespace core::type::convert;
        if constexpr (Signed) this->value.u.highPart = asS32(value);
        else this->value.u.highPart = asU32(value);
    }

    Napi::Value getBytes(const Napi::CallbackInfo& info) {
        return Napi::ArrayBuffer::New(info.Env(), value.bytes, 8);
    }

    Napi::Value neg(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart = -value.quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value add(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart += valueFrom(info[0]).quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value sub(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart -= valueFrom(info[0]).quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value mul(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart *= valueFrom(info[0]).quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value div(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart /= valueFrom(info[0]).quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value mod(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart %= valueFrom(info[0]).quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value band(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart &= valueFrom(info[0]).quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value bor(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart |= valueFrom(info[0]).quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value xor_(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart ^= valueFrom(info[0]).quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value not_(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart = ~value.quadPart;
        return create(info.Env(), newValue);
    }

    Napi::Value shl(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        auto newValue = value;
        newValue.quadPart <<= asS32(info[0]);
        return create(info.Env(), newValue);
    }

    Napi::Value shr(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        auto newValue = value;
        newValue.quadPart >>= asS32(info[0]);
        return create(info.Env(), newValue);
    }

    Napi::Value eq(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromBool(info.Env(), value.quadPart == valueFrom(info[0]).quadPart);
    }

    Napi::Value neq(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromBool(info.Env(), value.quadPart != valueFrom(info[0]).quadPart);
    }

    Napi::Value lt(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromBool(info.Env(), value.quadPart < valueFrom(info[0]).quadPart);
    }

    Napi::Value lte(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromBool(info.Env(), value.quadPart <= valueFrom(info[0]).quadPart);
    }

    Napi::Value gt(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromBool(info.Env(), value.quadPart > valueFrom(info[0]).quadPart);
    }

    Napi::Value gte(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromBool(info.Env(), value.quadPart >= valueFrom(info[0]).quadPart);
    }

    Napi::Value bswap(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;

        auto newValue = value;

        if (info.Length() == 0) {
            newValue.quadPart = static_cast<ValueQuadPartType>(bswap64(static_cast<std::uint64_t>(newValue.quadPart)));
        }
        else if (info.Length() == 1) {
            switch (/* width */ asS32(info[0])) {
            case 8: break;
            case 16:
                newValue.quadPart = static_cast<ValueQuadPartType>(bswap16(static_cast<std::uint16_t>(newValue.quadPart)));
                break;
            case 32:
                newValue.quadPart = static_cast<ValueQuadPartType>(bswap32(static_cast<std::uint32_t>(newValue.quadPart)));
                break;
            case 64:
                newValue.quadPart = static_cast<ValueQuadPartType>(bswap64(static_cast<std::uint64_t>(newValue.quadPart)));
                break;
            default:
                throw Napi::Error::New(info.Env(), "invalid width");
            }
        }

        return create(info.Env(), newValue);
    }

    Napi::Value bceil(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart = std::bit_ceil(static_cast<std::uint64_t>(newValue.quadPart));
        return create(info.Env(), newValue);
    }

    Napi::Value bfloor(const Napi::CallbackInfo& info) {
        auto newValue = value;
        newValue.quadPart = std::bit_floor(static_cast<std::uint64_t>(newValue.quadPart));
        return create(info.Env(), newValue);
    }

    Napi::Value bwidth(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromS32(info.Env(), std::bit_width(static_cast<std::uint64_t>(value.quadPart)));
    }

    Napi::Value rotl(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;

        auto newValue = value;
        const auto shift = asS32(info[0]);

        if (info.Length() == 1) {
            newValue.quadPart = std::rotl(static_cast<std::uint64_t>(newValue.quadPart), shift);
        }
        else if (info.Length() == 2) {
            switch (/* width */ asS32(info[1])) {
            case 8:
                newValue.quadPart = std::rotl(static_cast<std::uint8_t>(newValue.quadPart), shift);
                break;
            case 16:
                newValue.quadPart = std::rotl(static_cast<std::uint16_t>(newValue.quadPart), shift);
                break;
            case 32:
                newValue.quadPart = std::rotl(static_cast<std::uint32_t>(newValue.quadPart), shift);
                break;
            case 64:
                newValue.quadPart = std::rotl(static_cast<std::uint64_t>(newValue.quadPart), shift);
                break;
            default:
                throw Napi::Error::New(info.Env(), "invalid width");
            }
        }

        return create(info.Env(), newValue);
    }

    Napi::Value rotr(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;

        auto newValue = value;
        const auto shift = asS32(info[0]);

        if (info.Length() == 1) {
            newValue.quadPart = std::rotr(static_cast<std::uint64_t>(newValue.quadPart), shift);
        }
        else if (info.Length() == 2) {
            switch (/* width */ asS32(info[1])) {
            case 8:
                newValue.quadPart = std::rotr(static_cast<std::uint8_t>(newValue.quadPart), shift);
                break;
            case 16:
                newValue.quadPart = std::rotr(static_cast<std::uint16_t>(newValue.quadPart), shift);
                break;
            case 32:
                newValue.quadPart = std::rotr(static_cast<std::uint32_t>(newValue.quadPart), shift);
                break;
            case 64:
                newValue.quadPart = std::rotr(static_cast<std::uint64_t>(newValue.quadPart), shift);
                break;
            default:
                throw Napi::Error::New(info.Env(), "invalid width");
            }
        }

        return create(info.Env(), newValue);
    }

    Napi::Value countlZero(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromS32(info.Env(), std::countl_zero(static_cast<std::uint64_t>(value.quadPart)));
    }

    Napi::Value countlOne(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromS32(info.Env(), std::countl_one(static_cast<std::uint64_t>(value.quadPart)));
    }

    Napi::Value countrZero(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromS32(info.Env(), std::countr_zero(static_cast<std::uint64_t>(value.quadPart)));
    }

    Napi::Value countrOne(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromS32(info.Env(), std::countr_one(static_cast<std::uint64_t>(value.quadPart)));
    }

    Napi::Value popCount(const Napi::CallbackInfo& info) {
        using namespace core::type::convert;
        return fromS32(info.Env(), std::popcount(static_cast<std::uint64_t>(value.quadPart)));
    }

    static ValueType valueFromString(const Napi::Value& val, int base) {
        using namespace core::type::convert;

        ValueType newValue{};
        const auto str = asStrUtf8(val);

        if constexpr (Signed) {
            newValue.quadPart = std::strtoll(str.c_str(), nullptr, base);
            if (errno == ERANGE) throw Napi::Error::New(val.Env(), "couldn't convert string to long long");
        }
        else {
            newValue.quadPart = std::strtoull(str.c_str(), nullptr, base);
            if (errno == ERANGE) throw Napi::Error::New(val.Env(), "couldn't convert string to unsigned long long");
        }

        return newValue;
    }

    static ValueType valueFrom(const Napi::Value& val) {
        using namespace core::type;
        using namespace core::type::convert;

        ValueType newValue{};

        if (isInstanceOf<SignedLargeInteger>(val)) {
            newValue.quadPart = static_cast<ValueQuadPartType>(SignedLargeInteger::unwrap(val)->value.quadPart);
        }
        else if (isInstanceOf<UnsignedLargeInteger>(val)) {
            newValue.quadPart = static_cast<ValueQuadPartType>(UnsignedLargeInteger::unwrap(val)->value.quadPart);
        }
        else if (val.IsNumber()) {
            newValue.quadPart = static_cast<ValueQuadPartType>(asF64(val));
        }
        else if (val.IsString()) {
            return valueFromString(val, 10);
        }
        else if (val.IsBigInt()) {
            if constexpr (Signed) newValue.quadPart = asS64(val);
            else newValue.quadPart = asU64(val);
        }
        else if (val.IsArray()) {
            const auto v = valueAsArray(val);
            if (v.Length() == 2 && v.Get(0u).IsNumber() && v.Get(1u).IsNumber()) {
                newValue.u.lowPart = asU32(v.Get(0u));
                if constexpr (Signed) newValue.u.highPart = asS32(v.Get(1u));
                else newValue.u.highPart = asU32(v.Get(1u));
            }
            else if (v.Length() == 2 && v.Get(0u).IsString() && v.Get(1u).IsNumber()) {
                return valueFromString(v.Get(0u), asS32(v.Get(1u)));
            }
        }

        return newValue;
    }
};

}
