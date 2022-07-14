#pragma once

#include <vector>

#include <napi.h>

struct ImDrawList;

namespace imgui {

class DrawList final : public Napi::ObjectWrap<DrawList> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env, ImDrawList* val, ImVec2 origin);

    static DrawList* unwrap(const Napi::Object& obj) {
        return DrawList::Unwrap(obj);
    }

    static DrawList* unwrap(const Napi::Value& val) {
        return DrawList::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImDrawList* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImDrawList* get() {
        return m_val;
    }

    DrawList(const Napi::CallbackInfo& info);
    virtual ~DrawList() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImDrawList* m_val;
    ImVec2 m_origin;

    static std::vector<ImVec2> asPoints(const Napi::Value& val, ImVec2 origin);

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getFlags(const Napi::CallbackInfo& info);
    void setFlags(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value pushClipRect(const Napi::CallbackInfo& info);
    Napi::Value pushClipRectFullScreen(const Napi::CallbackInfo& info);
    Napi::Value popClipRect(const Napi::CallbackInfo& info);
    Napi::Value pushTextureId(const Napi::CallbackInfo& info);
    Napi::Value popTextureId(const Napi::CallbackInfo& info);
    Napi::Value getClipRectMin(const Napi::CallbackInfo& info);
    Napi::Value getClipRectMax(const Napi::CallbackInfo& info);
    Napi::Value addLine(const Napi::CallbackInfo& info);
    Napi::Value addRect(const Napi::CallbackInfo& info);
    Napi::Value addRectFilled(const Napi::CallbackInfo& info);
    Napi::Value addRectFilledMultiColor(const Napi::CallbackInfo& info);
    Napi::Value addQuad(const Napi::CallbackInfo& info);
    Napi::Value addQuadFilled(const Napi::CallbackInfo& info);
    Napi::Value addTriangle(const Napi::CallbackInfo& info);
    Napi::Value addTriangleFilled(const Napi::CallbackInfo& info);
    Napi::Value addCircle(const Napi::CallbackInfo& info);
    Napi::Value addCircleFilled(const Napi::CallbackInfo& info);
    Napi::Value addNgon(const Napi::CallbackInfo& info);
    Napi::Value addNgonFilled(const Napi::CallbackInfo& info);
    Napi::Value addText(const Napi::CallbackInfo& info);
    Napi::Value addPolyline(const Napi::CallbackInfo& info);
    Napi::Value addConvexPolyFilled(const Napi::CallbackInfo& info);
    Napi::Value addBezierCubic(const Napi::CallbackInfo& info);
    Napi::Value addBezierQuadratic(const Napi::CallbackInfo& info);
    Napi::Value addImage(const Napi::CallbackInfo& info);
    Napi::Value addImageQuad(const Napi::CallbackInfo& info);
    Napi::Value addImageRounded(const Napi::CallbackInfo& info);
    Napi::Value pathClear(const Napi::CallbackInfo& info);
    Napi::Value pathLineTo(const Napi::CallbackInfo& info);
    Napi::Value pathLineToMergeDuplicate(const Napi::CallbackInfo& info);
    Napi::Value pathFillConvex(const Napi::CallbackInfo& info);
    Napi::Value pathStroke(const Napi::CallbackInfo& info);
    Napi::Value pathArcTo(const Napi::CallbackInfo& info);
    Napi::Value pathArcToFast(const Napi::CallbackInfo& info);
    Napi::Value pathBezierCubicCurveTo(const Napi::CallbackInfo& info);
    Napi::Value pathBezierQuadraticCurveTo(const Napi::CallbackInfo& info);
    Napi::Value pathRect(const Napi::CallbackInfo& info);
    Napi::Value addDrawCmd(const Napi::CallbackInfo& info);
};

}
