#include "core/type/instanceOf.hpp"
#include "imgui/tableColumnSortSpecs.hpp"
#include "imgui/tableSortSpecs.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core::type::convert;

namespace imgui {

Napi::FunctionReference TableSortSpecs::m_ctor;

Napi::Object TableSortSpecs::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "TableSortSpecs";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&TableSortSpecs::getTypeId>("typeId"),
        InstanceAccessor<&TableSortSpecs::toDebugString>("debugString"),
        InstanceMethod<&TableSortSpecs::ref>("ref"),
        InstanceMethod<&TableSortSpecs::unref>("unref"),

        InstanceMethod<&TableSortSpecs::equals>("equals"),
        InstanceMethod<&TableSortSpecs::getSpecsAt>("getSpecsAt"),
        InstanceAccessor<&TableSortSpecs::getSpecsCount, &TableSortSpecs::setSpecsCount>("specsCount"),
        InstanceAccessor<&TableSortSpecs::getSpecsDirty, &TableSortSpecs::setSpecsDirty>("specsDirty")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object TableSortSpecs::create(Napi::Env env, ImGuiTableSortSpecs* val) {
    const auto obj = m_ctor.New({});
    unwrap(obj)->m_val = val;
    return obj;
}

TableSortSpecs::TableSortSpecs(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TableSortSpecs>{ info } {}

Napi::Value TableSortSpecs::getTypeId(const Napi::CallbackInfo& info) {
    return core::type::fromTypeId<TableSortSpecs>(info.Env());
}

Napi::Value TableSortSpecs::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value TableSortSpecs::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value TableSortSpecs::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value TableSortSpecs::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == TableSortSpecs::get(info[0]));
}

Napi::Value TableSortSpecs::getSpecsAt(const Napi::CallbackInfo& info) {
    return TableColumnSortSpecs::create(info.Env(), const_cast<ImGuiTableColumnSortSpecs*>(&m_val->Specs[/* index */ asS32(info[0])]));
}

Napi::Value TableSortSpecs::getSpecsCount(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->SpecsCount);
}

void TableSortSpecs::setSpecsCount(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->SpecsCount = asS32(val);
}

Napi::Value TableSortSpecs::getSpecsDirty(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->SpecsDirty);
}

void TableSortSpecs::setSpecsDirty(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->SpecsDirty = asBool(val);
}

}
