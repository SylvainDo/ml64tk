#include "core/largeInteger.hpp"
#include "core/opaquePointer.hpp"
#include "imgui/convert.hpp"
#include "imgui/memoryEditor.hpp"

#include <fmt/format.h>
#include <imgui_memory_editor.h>

using namespace core;
using namespace imgui;

namespace imgui {

Napi::FunctionReference MemoryEditor_::m_ctor;

Napi::Object MemoryEditor_::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "MemoryEditor";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&MemoryEditor_::getTypeId>("typeId"),
        InstanceAccessor<&MemoryEditor_::toDebugString>("debugString"),
        InstanceMethod<&MemoryEditor_::ref>("ref"),
        InstanceMethod<&MemoryEditor_::unref>("unref"),

        InstanceMethod<&MemoryEditor_::equals>("equals"),
        InstanceAccessor<&MemoryEditor_::getOpen, &MemoryEditor_::setOpen>("open"),
        InstanceAccessor<&MemoryEditor_::getReadOnly, &MemoryEditor_::setReadOnly>("readOnly"),
        InstanceAccessor<&MemoryEditor_::getCols, &MemoryEditor_::setCols>("cols"),
        InstanceAccessor<&MemoryEditor_::getOptShowOptions, &MemoryEditor_::setOptShowOptions>("optShowOptions"),
        InstanceAccessor<&MemoryEditor_::getOptShowDataPreview, &MemoryEditor_::setOptShowDataPreview>("optShowDataPreview"),
        InstanceAccessor<&MemoryEditor_::getOptShowHexII, &MemoryEditor_::setOptShowHexII>("optShowHexII"),
        InstanceAccessor<&MemoryEditor_::getOptShowAscii, &MemoryEditor_::setOptShowAscii>("optShowAscii"),
        InstanceAccessor<&MemoryEditor_::getOptGreyOutZeroes, &MemoryEditor_::setOptGreyOutZeroes>("optGreyOutZeroes"),
        InstanceAccessor<&MemoryEditor_::getOptUpperCaseHex, &MemoryEditor_::setOptUpperCaseHex>("optUpperCaseHex"),
        InstanceAccessor<&MemoryEditor_::getOptMidColsCount, &MemoryEditor_::setOptMidColsCount>("optMidColsCount"),
        InstanceAccessor<&MemoryEditor_::getOptAddrDigitsCount, &MemoryEditor_::setOptAddrDigitsCount>("optAddrDigitsCount"),
        InstanceAccessor<&MemoryEditor_::getOptFooterExtraHeight, &MemoryEditor_::setOptFooterExtraHeight>("optFooterExtraHeight"),
        InstanceAccessor<&MemoryEditor_::getHighlightColor, &MemoryEditor_::setHighlightColor>("highlightColor"),
        InstanceMethod<&MemoryEditor_::drawWindow>("drawWindow"),
        InstanceMethod<&MemoryEditor_::drawContents>("drawContents")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

MemoryEditor_::MemoryEditor_(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<MemoryEditor_>{ info },
    m_edit{ std::make_unique<MemoryEditor>() } {}

Napi::Value MemoryEditor_::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<MemoryEditor_>(info.Env());
}

Napi::Value MemoryEditor_::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("MemoryEditor ()"));
}

Napi::Value MemoryEditor_::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value MemoryEditor_::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value MemoryEditor_::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), this == MemoryEditor_::unwrap(info[0]));
}

Napi::Value MemoryEditor_::getOpen(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_edit->Open);
}

void MemoryEditor_::setOpen(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->Open = asBool(val);
}

Napi::Value MemoryEditor_::getReadOnly(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_edit->ReadOnly);
}

void MemoryEditor_::setReadOnly(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->ReadOnly = asBool(val);
}

Napi::Value MemoryEditor_::getCols(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_edit->Cols);
}

void MemoryEditor_::setCols(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->Cols = asS32(val);
}

Napi::Value MemoryEditor_::getOptShowOptions(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_edit->OptShowOptions);
}

void MemoryEditor_::setOptShowOptions(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->OptShowOptions = asBool(val);
}

Napi::Value MemoryEditor_::getOptShowDataPreview(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_edit->OptShowDataPreview);
}

void MemoryEditor_::setOptShowDataPreview(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->OptShowDataPreview = asBool(val);
}

Napi::Value MemoryEditor_::getOptShowHexII(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_edit->OptShowHexII);
}

void MemoryEditor_::setOptShowHexII(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->OptShowHexII = asBool(val);
}

Napi::Value MemoryEditor_::getOptShowAscii(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_edit->OptShowAscii);
}

void MemoryEditor_::setOptShowAscii(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->OptShowAscii = asBool(val);
}

Napi::Value MemoryEditor_::getOptGreyOutZeroes(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_edit->OptGreyOutZeroes);
}

void MemoryEditor_::setOptGreyOutZeroes(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->OptGreyOutZeroes = asBool(val);
}

Napi::Value MemoryEditor_::getOptUpperCaseHex(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_edit->OptUpperCaseHex);
}

void MemoryEditor_::setOptUpperCaseHex(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->OptUpperCaseHex = asBool(val);
}

Napi::Value MemoryEditor_::getOptMidColsCount(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_edit->OptMidColsCount);
}

void MemoryEditor_::setOptMidColsCount(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->OptMidColsCount = asS32(val);
}

Napi::Value MemoryEditor_::getOptAddrDigitsCount(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_edit->OptAddrDigitsCount);
}

void MemoryEditor_::setOptAddrDigitsCount(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->OptAddrDigitsCount = asS32(val);
}

Napi::Value MemoryEditor_::getOptFooterExtraHeight(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_edit->OptFooterExtraHeight);
}

void MemoryEditor_::setOptFooterExtraHeight(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->OptFooterExtraHeight = asF32(val);
}

Napi::Value MemoryEditor_::getHighlightColor(const Napi::CallbackInfo& info) {
    return fromColor(info.Env(), m_edit->HighlightColor);
}

void MemoryEditor_::setHighlightColor(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_edit->HighlightColor = ImGui::GetColorU32(asColor(val).Value);
}

Napi::Value MemoryEditor_::drawWindow(const Napi::CallbackInfo& info) {
    m_edit->DrawWindow(
        /* title */ asStrUtf8(info[0]).c_str(),
        /* mem_data */ isInstanceOf<OpaquePointer>(info[1]) ? OpaquePointer::get(info[1]) : info[1].As<Napi::ArrayBuffer>().Data(),
        /* mem_size */ UnsignedLargeInteger::get(info[2]),
        /* base_display_addr */ info[3].IsUndefined() ? 0 : UnsignedLargeInteger::get(info[3]));
    return info.Env().Undefined();
}

Napi::Value MemoryEditor_::drawContents(const Napi::CallbackInfo& info) {
    m_edit->DrawContents(
        /* mem_data */ isInstanceOf<OpaquePointer>(info[0]) ? OpaquePointer::get(info[0]) : info[0].As<Napi::ArrayBuffer>().Data(),
        /* mem_size */ UnsignedLargeInteger::get(info[1]),
        /* base_display_addr */ info[2].IsUndefined() ? 0 : UnsignedLargeInteger::get(info[2]));
    return info.Env().Undefined();
}

}
