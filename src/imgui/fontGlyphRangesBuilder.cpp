#include "core/instanceOf.hpp"
#include "imgui/fontGlyphRangesBuilder.hpp"

#include <fmt/format.h>

using namespace core;

namespace imgui {

Napi::FunctionReference FontGlyphRangesBuilder::m_ctor;

Napi::Object FontGlyphRangesBuilder::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "FontGlyphRangesBuilder";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&FontGlyphRangesBuilder::getTypeId>("typeId"),
        InstanceAccessor<&FontGlyphRangesBuilder::toDebugString>("debugString"),
        InstanceMethod<&FontGlyphRangesBuilder::ref>("ref"),
        InstanceMethod<&FontGlyphRangesBuilder::unref>("unref"),

        InstanceMethod<&FontGlyphRangesBuilder::equals>("equals"),
        InstanceMethod<&FontGlyphRangesBuilder::clear>("clear"),
        InstanceMethod<&FontGlyphRangesBuilder::getBit>("getBit"),
        InstanceMethod<&FontGlyphRangesBuilder::setBit>("setBit"),
        InstanceMethod<&FontGlyphRangesBuilder::addChar>("addChar"),
        InstanceMethod<&FontGlyphRangesBuilder::addText>("addText"),
        InstanceMethod<&FontGlyphRangesBuilder::addRanges>("addRanges"),
        InstanceMethod<&FontGlyphRangesBuilder::buildRanges>("build")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object FontGlyphRangesBuilder::create(Napi::Env) {
    return m_ctor.New({});
}

FontGlyphRangesBuilder::FontGlyphRangesBuilder(const Napi::CallbackInfo& info) : Napi::ObjectWrap<FontGlyphRangesBuilder>{ info } {}

Napi::Value FontGlyphRangesBuilder::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<FontGlyphRangesBuilder>(info.Env());
}

Napi::Value FontGlyphRangesBuilder::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("ImGui.FontGlyphRangesBuilder (this={}; instance={})",
        fmt::ptr(this), fmt::ptr(&m_val)));
}

Napi::Value FontGlyphRangesBuilder::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value FontGlyphRangesBuilder::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value FontGlyphRangesBuilder::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), &m_val == FontGlyphRangesBuilder::get(info[0]));
}

Napi::Value FontGlyphRangesBuilder::clear(const Napi::CallbackInfo& info) {
    m_val.Clear();
    return info.Env().Undefined();
}

Napi::Value FontGlyphRangesBuilder::getBit(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val.GetBit(/* n */ asSize(info[0])));
}

Napi::Value FontGlyphRangesBuilder::setBit(const Napi::CallbackInfo& info) {
    m_val.SetBit(/* n */ asSize(info[0]));
    return info.Env().Undefined();
}

Napi::Value FontGlyphRangesBuilder::addChar(const Napi::CallbackInfo& info) {
    m_val.AddChar(/* c */ asU16(info[0]));
    return info.Env().Undefined();
}

Napi::Value FontGlyphRangesBuilder::addText(const Napi::CallbackInfo& info) {
    m_val.AddText(/* text */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value FontGlyphRangesBuilder::addRanges(const Napi::CallbackInfo& info) {
    m_val.AddRanges(/* ranges */ info[0].As<Napi::Uint16Array>().Data());
    return info.Env().Undefined();
}

Napi::Value FontGlyphRangesBuilder::buildRanges(const Napi::CallbackInfo& info) {
    ImVector<ImWchar> v;
    m_val.BuildRanges(&v);
    auto ranges = Napi::Uint16Array::New(info.Env(), v.Size);
    std::memcpy(ranges.Data(), v.Data, sizeof(ImWchar) * v.Size);
    return ranges;
}

}
