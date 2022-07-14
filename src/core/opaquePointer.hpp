#pragma once

#include <napi.h>

namespace core {

class OpaquePointer final : public Napi::ObjectWrap<OpaquePointer> {
public:
    enum class Type {
        Raw,
        ObjectRef
    };

    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env, void* val, Type type = Type::Raw);
    static Napi::Object create(Napi::Env, const Napi::Object& val);

    static OpaquePointer* unwrap(const Napi::Object& obj) {
        return OpaquePointer::Unwrap(obj);
    }

    static OpaquePointer* unwrap(const Napi::Value& val) {
        return OpaquePointer::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static void* get(const T& val) {
        return unwrap(val)->m_val;
    }

    void* get() {
        return m_val;
    }

    OpaquePointer(const Napi::CallbackInfo& info);
    virtual ~OpaquePointer();

private:
    static Napi::FunctionReference m_ctor;
    void* m_val;
    Type m_type;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    static Napi::Value fromPointer(const Napi::CallbackInfo& info);
    static Napi::Value fromObject(const Napi::CallbackInfo& info);
    Napi::Value getRawPointer(const Napi::CallbackInfo& info);
    Napi::Value asPointer(const Napi::CallbackInfo& info);
    Napi::Value asObject(const Napi::CallbackInfo& info);
    void releaseReal();
    Napi::Value release(const Napi::CallbackInfo& info);
};

}
