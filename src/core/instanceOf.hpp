#pragma once

#include "core/convert.hpp"

#include <typeinfo>

namespace core {

template <class T>
bool isInstanceOf(const Napi::Value& value) {
    if (!value.IsObject()) return false;
    const auto obj = valueAsObject(value);
    if (!obj.Has("typeId")) return false;
    return asSize(obj.Get("typeId")) == typeid(T).hash_code();
}

template <class T>
Napi::Value fromTypeId(Napi::Env env) {
    return fromSize(env, typeid(T).hash_code());
}

}
