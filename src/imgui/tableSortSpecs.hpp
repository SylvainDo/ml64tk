#pragma once

#include <napi.h>

struct ImGuiTableSortSpecs;

namespace imgui {

class TableSortSpecs final : public Napi::ObjectWrap<TableSortSpecs> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env, ImGuiTableSortSpecs* val);

    static TableSortSpecs* unwrap(const Napi::Object& obj) {
        return TableSortSpecs::Unwrap(obj);
    }

    static TableSortSpecs* unwrap(const Napi::Value& val) {
        return TableSortSpecs::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiTableSortSpecs* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiTableSortSpecs* get() {
        return m_val;
    }

    TableSortSpecs(const Napi::CallbackInfo& info);
    virtual ~TableSortSpecs() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImGuiTableSortSpecs* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getSpecsAt(const Napi::CallbackInfo& info);
    Napi::Value getSpecsCount(const Napi::CallbackInfo& info);
    void setSpecsCount(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getSpecsDirty(const Napi::CallbackInfo& info);
    void setSpecsDirty(const Napi::CallbackInfo&, const Napi::Value& val);
};

}
