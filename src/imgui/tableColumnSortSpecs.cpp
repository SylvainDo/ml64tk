#include "core/type/instanceOf.hpp"
#include "imgui/tableColumnSortSpecs.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core::type::convert;

namespace imgui {

Napi::FunctionReference TableColumnSortSpecs::m_ctor;

Napi::Object TableColumnSortSpecs::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "TableColumnSortSpecs";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&TableColumnSortSpecs::getTypeId>("typeId"),
        InstanceAccessor<&TableColumnSortSpecs::toDebugString>("debugString"),
        InstanceMethod<&TableColumnSortSpecs::ref>("ref"),
        InstanceMethod<&TableColumnSortSpecs::unref>("unref"),

        InstanceMethod<&TableColumnSortSpecs::equals>("equals"),
        InstanceAccessor<&TableColumnSortSpecs::getColumnUserId, &TableColumnSortSpecs::setColumnUserId>("columnUserId"),
        InstanceAccessor<&TableColumnSortSpecs::getColumnIndex, &TableColumnSortSpecs::setColumnIndex>("columnIndex"),
        InstanceAccessor<&TableColumnSortSpecs::getSortOrder, &TableColumnSortSpecs::setSortOrder>("sortOrder"),
        InstanceAccessor<&TableColumnSortSpecs::getSortDirection, &TableColumnSortSpecs::setSortDirection>("sortDirection")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object TableColumnSortSpecs::create(Napi::Env env, ImGuiTableColumnSortSpecs* val) {
    const auto obj = m_ctor.New({});
    unwrap(obj)->m_val = val;
    return obj;
}

TableColumnSortSpecs::TableColumnSortSpecs(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TableColumnSortSpecs>{ info } {}

Napi::Value TableColumnSortSpecs::getTypeId(const Napi::CallbackInfo& info) {
    return core::type::fromTypeId<TableColumnSortSpecs>(info.Env());
}

Napi::Value TableColumnSortSpecs::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value TableColumnSortSpecs::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value TableColumnSortSpecs::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value TableColumnSortSpecs::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == TableColumnSortSpecs::get(info[0]));
}

Napi::Value TableColumnSortSpecs::getColumnUserId(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_val->ColumnUserID);
}

void TableColumnSortSpecs::setColumnUserId(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ColumnUserID = asU32(val);
}

Napi::Value TableColumnSortSpecs::getColumnIndex(const Napi::CallbackInfo& info) {
    return fromS16(info.Env(), m_val->ColumnIndex);
}

void TableColumnSortSpecs::setColumnIndex(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ColumnIndex = asS16(val);
}

Napi::Value TableColumnSortSpecs::getSortOrder(const Napi::CallbackInfo& info) {
    return fromS16(info.Env(), m_val->SortOrder);
}

void TableColumnSortSpecs::setSortOrder(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->SortOrder = asS16(val);
}

Napi::Value TableColumnSortSpecs::getSortDirection(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->SortDirection);
}

void TableColumnSortSpecs::setSortDirection(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->SortDirection = asS32(val);
}

}
