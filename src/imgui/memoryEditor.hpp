#pragma once

#include <memory>

#include <napi.h>

struct MemoryEditor;

namespace imgui {

class MemoryEditor_ final : public Napi::ObjectWrap<MemoryEditor_> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);

    static MemoryEditor_* unwrap(const Napi::Object& obj) {
        return MemoryEditor_::Unwrap(obj);
    }

    static MemoryEditor_* unwrap(const Napi::Value& val) {
        return MemoryEditor_::Unwrap(val.As<Napi::Object>());
    }

    MemoryEditor_(const Napi::CallbackInfo& info);
    virtual ~MemoryEditor_() = default;

private:
    static Napi::FunctionReference m_ctor;
    std::unique_ptr<::MemoryEditor> m_edit;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getOpen(const Napi::CallbackInfo& info);
    void setOpen(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getReadOnly(const Napi::CallbackInfo& info);
    void setReadOnly(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getCols(const Napi::CallbackInfo& info);
    void setCols(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOptShowOptions(const Napi::CallbackInfo& info);
    void setOptShowOptions(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOptShowDataPreview(const Napi::CallbackInfo& info);
    void setOptShowDataPreview(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOptShowHexII(const Napi::CallbackInfo& info);
    void setOptShowHexII(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOptShowAscii(const Napi::CallbackInfo& info);
    void setOptShowAscii(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOptGreyOutZeroes(const Napi::CallbackInfo& info);
    void setOptGreyOutZeroes(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOptUpperCaseHex(const Napi::CallbackInfo& info);
    void setOptUpperCaseHex(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOptMidColsCount(const Napi::CallbackInfo& info);
    void setOptMidColsCount(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOptAddrDigitsCount(const Napi::CallbackInfo& info);
    void setOptAddrDigitsCount(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getOptFooterExtraHeight(const Napi::CallbackInfo& info);
    void setOptFooterExtraHeight(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value getHighlightColor(const Napi::CallbackInfo& info);
    void setHighlightColor(const Napi::CallbackInfo& info, const Napi::Value& val);
    Napi::Value drawWindow(const Napi::CallbackInfo& info);
    Napi::Value drawContents(const Napi::CallbackInfo& info);
};

}
