#pragma once

#include <napi.h>

struct ImGuiTableColumnSortSpecs;

namespace imgui {

class TableColumnSortSpecs final : public Napi::ObjectWrap<TableColumnSortSpecs> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env, ImGuiTableColumnSortSpecs* val);

    static TableColumnSortSpecs* unwrap(const Napi::Object& obj) {
        return TableColumnSortSpecs::Unwrap(obj);
    }

    static TableColumnSortSpecs* unwrap(const Napi::Value& val) {
        return TableColumnSortSpecs::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiTableColumnSortSpecs* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiTableColumnSortSpecs* get() {
        return m_val;
    }

    TableColumnSortSpecs(const Napi::CallbackInfo& info);
    virtual ~TableColumnSortSpecs() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImGuiTableColumnSortSpecs* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getColumnUserId(const Napi::CallbackInfo& info);
    void setColumnUserId(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getColumnIndex(const Napi::CallbackInfo& info);
    void setColumnIndex(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getSortOrder(const Napi::CallbackInfo& info);
    void setSortOrder(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getSortDirection(const Napi::CallbackInfo& info);
    void setSortDirection(const Napi::CallbackInfo&, const Napi::Value& val);
};

}
