#pragma once

#include "core/type/convert.hpp"

#include <typeinfo>

namespace core::type {

template <class T>
bool isInstanceOf(const Napi::Value& value) {
    using namespace convert;
    if (!value.IsObject()) return false;
    const auto obj = valueAsObject(value);
    if (!obj.Has("typeId")) return false;
    return asSize(obj.Get("typeId")) == typeid(T).hash_code();
}

}
