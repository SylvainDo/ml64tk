#include "core/type/instanceOf.hpp"
#include "core/opaquePointer.hpp"
#include "imgui/convert.hpp"
#include "imgui/drawList.hpp"
#include "imgui/font.hpp"
#include "texture.hpp"

#include <cmath>
#include <array>
#include <optional>

#include <fmt/format.h>
#include <imgui.h>
#include <SDL_render.h>

using namespace core;
using namespace core::type::convert;

namespace imgui {

inline ImVec2 operator+(ImVec2 lhs, ImVec2 rhs) {
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

Napi::FunctionReference DrawList::m_ctor;

Napi::Object DrawList::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "DrawList";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&DrawList::getTypeId>("typeId"),
        InstanceAccessor<&DrawList::toDebugString>("debugString"),
        InstanceMethod<&DrawList::ref>("ref"),
        InstanceMethod<&DrawList::unref>("unref"),

        InstanceMethod<&DrawList::equals>("equals"),
        InstanceAccessor<&DrawList::getFlags, &DrawList::setFlags>("flags"),
        InstanceMethod<&DrawList::pushClipRect>("pushClipRect"),
        InstanceMethod<&DrawList::pushClipRectFullScreen>("pushClipRectFullScreen"),
        InstanceMethod<&DrawList::popClipRect>("popClipRect"),
        InstanceMethod<&DrawList::pushTextureId>("pushTextureId"),
        InstanceMethod<&DrawList::popTextureId>("popTextureId"),
        InstanceMethod<&DrawList::getClipRectMin>("getClipRectMin"),
        InstanceMethod<&DrawList::getClipRectMax>("getClipRectMax"),
        InstanceMethod<&DrawList::addLine>("addLine"),
        InstanceMethod<&DrawList::addRect>("addRect"),
        InstanceMethod<&DrawList::addRectFilled>("addRectFilled"),
        InstanceMethod<&DrawList::addRectFilledMultiColor>("addRectFilledMultiColor"),
        InstanceMethod<&DrawList::addQuad>("addQuad"),
        InstanceMethod<&DrawList::addQuadFilled>("addQuadFilled"),
        InstanceMethod<&DrawList::addTriangle>("addTriangle"),
        InstanceMethod<&DrawList::addTriangleFilled>("addTriangleFilled"),
        InstanceMethod<&DrawList::addCircle>("addCircle"),
        InstanceMethod<&DrawList::addCircleFilled>("addCircleFilled"),
        InstanceMethod<&DrawList::addNgon>("addNgon"),
        InstanceMethod<&DrawList::addNgonFilled>("addNgonFilled"),
        InstanceMethod<&DrawList::addText>("addText"),
        InstanceMethod<&DrawList::addPolyline>("addPolyline"),
        InstanceMethod<&DrawList::addConvexPolyFilled>("addConvexPolyFilled"),
        InstanceMethod<&DrawList::addBezierCubic>("addBezierCubic"),
        InstanceMethod<&DrawList::addBezierQuadratic>("addBezierQuadratic"),
        InstanceMethod<&DrawList::addImage>("addImage"),
        InstanceMethod<&DrawList::addImageQuad>("addImageQuad"),
        InstanceMethod<&DrawList::addImageRounded>("addImageRounded"),
        InstanceMethod<&DrawList::pathClear>("pathClear"),
        InstanceMethod<&DrawList::pathLineTo>("pathLineTo"),
        InstanceMethod<&DrawList::pathLineToMergeDuplicate>("pathLineToMergeDuplicate"),
        InstanceMethod<&DrawList::pathFillConvex>("pathFillConvex"),
        InstanceMethod<&DrawList::pathStroke>("pathStroke"),
        InstanceMethod<&DrawList::pathArcTo>("pathArcTo"),
        InstanceMethod<&DrawList::pathArcToFast>("pathArcToFast"),
        InstanceMethod<&DrawList::pathBezierCubicCurveTo>("pathBezierCubicCurveTo"),
        InstanceMethod<&DrawList::pathBezierQuadraticCurveTo>("pathBezierQuadraticCurveTo"),
        InstanceMethod<&DrawList::pathRect>("pathRect"),
        InstanceMethod<&DrawList::addDrawCmd>("addDrawCmd"),
        InstanceMethod<&DrawList::addSprite>("addSprite"),
        InstanceMethod<&DrawList::addSpriteEx>("addSpriteEx")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object DrawList::create(Napi::Env, ImDrawList* val, ImVec2 origin) {
    const auto obj = m_ctor.New({});
    auto instance = unwrap(obj);
    instance->m_val = val;
    instance->m_origin = origin;
    return obj;
}

DrawList::DrawList(const Napi::CallbackInfo& info) : Napi::ObjectWrap<DrawList>{ info } {}

std::vector<ImVec2> DrawList::asPoints(const Napi::Value& val, ImVec2 origin) {
    const auto a = valueAsArray(val);
    std::vector<ImVec2> v(a.Length());
    for (std::size_t i{}; i < v.size(); ++i)
        v[i] = asVec2(a.Get(static_cast<std::uint32_t>(i))) + origin;
    return v;
}

Napi::Value DrawList::getTypeId(const Napi::CallbackInfo& info) {
    return type::fromTypeId<DrawList>(info.Env());
}

Napi::Value DrawList::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value DrawList::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value DrawList::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value DrawList::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == DrawList::get(info[0]));
}

Napi::Value DrawList::getFlags(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->Flags);
}

void DrawList::setFlags(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->Flags = asS32(val);
}

Napi::Value DrawList::pushClipRect(const Napi::CallbackInfo& info) {
    m_val->PushClipRect(
        /* clip_rect_min */ asVec2(info[0]),
        /* clip_rect_max */ asVec2(info[1]),
        /* intersect_with_current_clip_rect */ asBoolOr(info[2], false));
    return info.Env().Undefined();
}

Napi::Value DrawList::pushClipRectFullScreen(const Napi::CallbackInfo& info) {
    m_val->PushClipRectFullScreen();
    return info.Env().Undefined();
}

Napi::Value DrawList::popClipRect(const Napi::CallbackInfo& info) {
    m_val->PopClipRect();
    return info.Env().Undefined();
}

Napi::Value DrawList::pushTextureId(const Napi::CallbackInfo& info) {
    m_val->PushTextureID(/* texture_id */ OpaquePointer::get(info[0]));
    return info.Env().Undefined();
}

Napi::Value DrawList::popTextureId(const Napi::CallbackInfo& info) {
    m_val->PopTextureID();
    return info.Env().Undefined();
}

Napi::Value DrawList::getClipRectMin(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->GetClipRectMin());
}

Napi::Value DrawList::getClipRectMax(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->GetClipRectMax());
}

Napi::Value DrawList::addLine(const Napi::CallbackInfo& info) {
    m_val->AddLine(
        /* p1 */ m_origin + asVec2(info[0]),
        /* p2 */ m_origin + asVec2(info[1]),
        /* col */ asColor(info[2]),
        /* thickness */ asF32Or(info[3], 1.0f));
    return info.Env().Undefined();
}

Napi::Value DrawList::addRect(const Napi::CallbackInfo& info) {
    m_val->AddRect(
        /* p_min */ m_origin + asVec2(info[0]),
        /* p_max */ m_origin + asVec2(info[1]),
        /* col */ asColor(info[2]),
        /* rounding */ asF32Or(info[3], 0.0f),
        /* flags */ asS32Or(info[4], 0),
        /* thickness */ asF32Or(info[5], 1.0f));
    return info.Env().Undefined();
}

Napi::Value DrawList::addRectFilled(const Napi::CallbackInfo& info) {
    m_val->AddRectFilled(
        /* p_min */ m_origin + asVec2(info[0]),
        /* p_max */ m_origin + asVec2(info[1]),
        /* col */ asColor(info[2]),
        /* rounding */ asF32Or(info[3], 0.0f),
        /* flags */ asS32Or(info[4], 0));
    return info.Env().Undefined();
}

Napi::Value DrawList::addRectFilledMultiColor(const Napi::CallbackInfo& info) {
    m_val->AddRectFilledMultiColor(
        /* p_min */ m_origin + asVec2(info[0]),
        /* p_max */ m_origin + asVec2(info[1]),
        /* col_upr_left */ asColor(info[2]),
        /* col_upr_right */ asColor(info[3]),
        /* col_bot_right */ asColor(info[4]),
        /* col_bot_left */ asColor(info[5]));
    return info.Env().Undefined();
}

Napi::Value DrawList::addQuad(const Napi::CallbackInfo& info) {
    m_val->AddQuad(
        /* p1 */ m_origin + asVec2(info[0]),
        /* p2 */ m_origin + asVec2(info[1]),
        /* p3 */ m_origin + asVec2(info[2]),
        /* p4 */ m_origin + asVec2(info[3]),
        /* col */ asColor(info[4]),
        /* thickness */ asF32Or(info[5], 1.0f));
    return info.Env().Undefined();
}

Napi::Value DrawList::addQuadFilled(const Napi::CallbackInfo& info) {
    m_val->AddQuadFilled(
        /* p1 */ m_origin + asVec2(info[0]),
        /* p2 */ m_origin + asVec2(info[1]),
        /* p3 */ m_origin + asVec2(info[2]),
        /* p4 */ m_origin + asVec2(info[3]),
        /* col */ asColor(info[4]));
    return info.Env().Undefined();
}

Napi::Value DrawList::addTriangle(const Napi::CallbackInfo& info) {
    m_val->AddTriangle(
        /* p1 */ m_origin + asVec2(info[0]),
        /* p2 */ m_origin + asVec2(info[1]),
        /* p3 */ m_origin + asVec2(info[2]),
        /* col */ asColor(info[3]),
        /* thickness */ asF32Or(info[4], 1.0f));
    return info.Env().Undefined();
}

Napi::Value DrawList::addTriangleFilled(const Napi::CallbackInfo& info) {
    m_val->AddTriangleFilled(
        /* p1 */ m_origin + asVec2(info[0]),
        /* p2 */ m_origin + asVec2(info[1]),
        /* p3 */ m_origin + asVec2(info[2]),
        /* col */ asColor(info[3]));
    return info.Env().Undefined();
}

Napi::Value DrawList::addCircle(const Napi::CallbackInfo& info) {
    m_val->AddCircle(
        /* center */ m_origin + asVec2(info[0]),
        /* radius */ asF32(info[1]),
        /* col */ asColor(info[2]),
        /* num_segments */ asS32Or(info[3], 0),
        /* thickness */ asF32Or(info[4], 1.0f));
    return info.Env().Undefined();
}

Napi::Value DrawList::addCircleFilled(const Napi::CallbackInfo& info) {
    m_val->AddCircleFilled(
        /* center */ m_origin + asVec2(info[0]),
        /* radius */ asF32(info[1]),
        /* col */ asColor(info[2]),
        /* num_segments */ asS32Or(info[3], 0));
    return info.Env().Undefined();
}

Napi::Value DrawList::addNgon(const Napi::CallbackInfo& info) {
    m_val->AddNgon(
        /* center */ m_origin + asVec2(info[0]),
        /* radius */ asF32(info[1]),
        /* col */ asColor(info[2]),
        /* num_segments */ asS32(info[3]),
        /* thickness */ asF32Or(info[4], 1.0f));
    return info.Env().Undefined();
}

Napi::Value DrawList::addNgonFilled(const Napi::CallbackInfo& info) {
    m_val->AddNgonFilled(
        /* center */ m_origin + asVec2(info[0]),
        /* radius */ asF32(info[1]),
        /* col */ asColor(info[2]),
        /* num_segments */ asS32(info[3]));
    return info.Env().Undefined();
}

Napi::Value DrawList::addText(const Napi::CallbackInfo& info) {
    if (type::isInstanceOf<Font>(info[0])) {
        const auto cpuFineClipRect = info[6].IsUndefined() ? std::nullopt : std::optional{ asVec4(info[6]) };
        m_val->AddText(
            /* font */ Font::get(info[0]),
            /* font_size */ asF32(info[1]),
            /* pos */ m_origin + asVec2(info[2]),
            /* col */ asColor(info[3]),
            /* text */ asStrUtf8(info[4]).c_str(), nullptr,
            /* wrap_width */ asF32(info[5]),
            /* cpu_fine_clip_rect */ cpuFineClipRect.has_value() ? &cpuFineClipRect.value() : nullptr);
    }
    else {
        m_val->AddText(
            /* pos */ m_origin + asVec2(info[0]),
            /* col */ asColor(info[1]),
            /* text */ asStrUtf8(info[2]).c_str());
    }
    return info.Env().Undefined();
}

Napi::Value DrawList::addPolyline(const Napi::CallbackInfo& info) {
    const auto points = asPoints(info[0], m_origin);
    m_val->AddPolyline(
        /* points */ points.data(),
        /* num_points */ points.size(),
        /* col */ asColor(info[1]),
        /* flags */ asS32Or(info[2], 0),
        /* thickness */ asF32Or(info[3], 1.0f));
    return info.Env().Undefined();
}

Napi::Value DrawList::addConvexPolyFilled(const Napi::CallbackInfo& info) {
    const auto points = asPoints(info[0], m_origin);
    m_val->AddConvexPolyFilled(
        /* points */ points.data(),
        /* num_points */ points.size(),
        /* col */ asColor(info[1]));
    return info.Env().Undefined();
}

Napi::Value DrawList::addBezierCubic(const Napi::CallbackInfo& info) {
    m_val->AddBezierCubic(
        /* p1 */ m_origin + asVec2(info[0]),
        /* p2 */ m_origin + asVec2(info[1]),
        /* p3 */ m_origin + asVec2(info[2]),
        /* p4 */ m_origin + asVec2(info[3]),
        /* col */ asColor(info[4]),
        /* thickness */ asF32Or(info[5], 1.0f),
        /* num_segments */ asS32Or(info[6], 0));
    return info.Env().Undefined();
}

Napi::Value DrawList::addBezierQuadratic(const Napi::CallbackInfo& info) {
    m_val->AddBezierQuadratic(
        /* p1 */ m_origin + asVec2(info[0]),
        /* p2 */ m_origin + asVec2(info[1]),
        /* p3 */ m_origin + asVec2(info[2]),
        /* col */ asColor(info[3]),
        /* thickness */ asF32Or(info[4], 1.0f),
        /* num_segments */ asS32Or(info[5], 0));
    return info.Env().Undefined();
}

Napi::Value DrawList::addImage(const Napi::CallbackInfo& info) {
    m_val->AddImage(
        /* user_texture_id */ OpaquePointer::get(info[0]),
        /* p_min */ m_origin + asVec2(info[1]),
        /* p_max */ m_origin + asVec2(info[2]),
        /* uv_min */ asVec2Or(info[3], {}),
        /* uv_max */ asVec2Or(info[4], { 1.0f, 1.0f }),
        /* col */ asColorOr(info[5], IM_COL32_WHITE));
    return info.Env().Undefined();
}

Napi::Value DrawList::addImageQuad(const Napi::CallbackInfo& info) {
    m_val->AddImageQuad(
        /* user_texture_id */ OpaquePointer::get(info[0]),
        /* p1 */ m_origin + asVec2(info[1]),
        /* p2 */ m_origin + asVec2(info[2]),
        /* p3 */ m_origin + asVec2(info[3]),
        /* p4 */ m_origin + asVec2(info[4]),
        /* uv1 */ asVec2Or(info[5], {}),
        /* uv2 */ asVec2Or(info[6], { 1.0f, 0.0f }),
        /* uv3 */ asVec2Or(info[7], { 1.0f, 1.0f }),
        /* uv4 */ asVec2Or(info[8], { 0.0f, 1.0f }),
        /* col */ asColorOr(info[9], IM_COL32_WHITE));
    return info.Env().Undefined();
}

Napi::Value DrawList::addImageRounded(const Napi::CallbackInfo& info) {
    m_val->AddImageRounded(
        /* user_texture_id */ OpaquePointer::get(info[0]),
        /* p_min */ m_origin + asVec2(info[1]),
        /* p_max */ m_origin + asVec2(info[2]),
        /* uv_min */ asVec2Or(info[3], {}),
        /* uv_max */ asVec2Or(info[4], { 1.0f, 1.0f }),
        /* col */ asColorOr(info[5], IM_COL32_WHITE),
        /* rounding */ asF32Or(info[6], 0.0f),
        /* flags */ asS32Or(info[7], 0));
    return info.Env().Undefined();
}

Napi::Value DrawList::pathClear(const Napi::CallbackInfo& info) {
    m_val->PathClear();
    return info.Env().Undefined();
}

Napi::Value DrawList::pathLineTo(const Napi::CallbackInfo& info) {
    m_val->PathLineTo(/* pos */ m_origin + asVec2(info[0]));
    return info.Env().Undefined();
}

Napi::Value DrawList::pathLineToMergeDuplicate(const Napi::CallbackInfo& info) {
    m_val->PathLineToMergeDuplicate(/* pos */ m_origin + asVec2(info[0]));
    return info.Env().Undefined();
}

Napi::Value DrawList::pathFillConvex(const Napi::CallbackInfo& info) {
    m_val->PathFillConvex(/* col */ asColor(info[0]));
    return info.Env().Undefined();
}

Napi::Value DrawList::pathStroke(const Napi::CallbackInfo& info) {
    m_val->PathStroke(
        /* col */ asColor(info[0]),
        /* flags */ asS32Or(info[1], 0),
        /* thickness */ asF32Or(info[2], 1.0f));
    return info.Env().Undefined();
}

Napi::Value DrawList::pathArcTo(const Napi::CallbackInfo& info) {
    m_val->PathArcTo(
        /* center */ m_origin + asVec2(info[0]),
        /* radius */ asF32(info[1]),
        /* a_min */ asF32(info[2]),
        /* a_max */ asF32(info[3]),
        /* num_segments */ asS32Or(info[4], 0));
    return info.Env().Undefined();
}

Napi::Value DrawList::pathArcToFast(const Napi::CallbackInfo& info) {
    m_val->PathArcToFast(
        /* center */ m_origin + asVec2(info[0]),
        /* radius */ asF32(info[1]),
        /* a_min_of_12 */ asS32(info[2]),
        /* a_max_of_12 */ asS32(info[3]));
    return info.Env().Undefined();
}

Napi::Value DrawList::pathBezierCubicCurveTo(const Napi::CallbackInfo& info) {
    m_val->PathBezierCubicCurveTo(
        /* p2 */ m_origin + asVec2(info[0]),
        /* p3 */ m_origin + asVec2(info[1]),
        /* p4 */ m_origin + asVec2(info[2]),
        /* num_segments */ asS32Or(info[3], 0));
    return info.Env().Undefined();
}

Napi::Value DrawList::pathBezierQuadraticCurveTo(const Napi::CallbackInfo& info) {
    m_val->PathBezierQuadraticCurveTo(
        /* p2 */ m_origin + asVec2(info[0]),
        /* p3 */ m_origin + asVec2(info[1]),
        /* num_segments */ asS32Or(info[2], 0));
    return info.Env().Undefined();
}

Napi::Value DrawList::pathRect(const Napi::CallbackInfo& info) {
    m_val->PathRect(
        /* rect_min */ m_origin + asVec2(info[0]),
        /* rect_max */ m_origin + asVec2(info[1]),
        /* rounding */ asF32Or(info[2], 0.0f),
        /* flags */ asS32Or(info[3], 0));
    return info.Env().Undefined();
}

Napi::Value DrawList::addDrawCmd(const Napi::CallbackInfo& info) {
    m_val->AddDrawCmd();
    return info.Env().Undefined();
}

Napi::Value DrawList::addSprite(const Napi::CallbackInfo& info) {
    const auto texture = Texture::unwrap(info[0]);
    const auto pos = m_origin + asVec2(info[1]);
    m_val->AddImage(texture->getIdPtr(), pos, pos + ImVec2(texture->getWidth(), texture->getHeight()));
    return info.Env().Undefined();
}

static std::array<ImVec2, 4> buildUVs(int textureWidth, int textureHeight, const ImVec4& src, SDL_RendererFlip flip) {
    std::array<ImVec2, 4> uv{
        ImVec2{ src.x / textureWidth, src.y / textureHeight },
        { (src.x + src.z) / textureWidth, src.y / textureHeight },
        { (src.x + src.z) / textureWidth, (src.y + src.w) / textureHeight },
        { src.x / textureWidth, (src.y + src.w) / textureHeight }
    };
    if ((flip & SDL_FLIP_HORIZONTAL) == SDL_FLIP_HORIZONTAL) {
        std::swap(uv[0], uv[1]);
        std::swap(uv[2], uv[3]);
    }
    if ((flip & SDL_FLIP_VERTICAL) == SDL_FLIP_VERTICAL) {
        std::swap(uv[0], uv[3]);
        std::swap(uv[1], uv[2]);
    }
    return uv;
}

inline ImVec2 rotateVec2(ImVec2 v, float cos_a, float sin_a) {
    return { v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a };
}

inline ImVec2 truncVec2(ImVec2 v) {
    return { std::trunc(v.x), std::trunc(v.y) };
}

Napi::Value DrawList::addSpriteEx(const Napi::CallbackInfo& info) {
    const auto texture = Texture::unwrap(info[0]);
    ImVec4 src(0.0f, 0.0f, texture->getWidth(), texture->getHeight());
    auto dst = src;
    ImColor tint{ IM_COL32_WHITE};
    auto flip = SDL_FLIP_NONE;
    float angle = 0.0f;
    ImVec2 offset{}, scale{ 1.0f, 1.0f };
    bool truncVtx{};
    std::array<ImVec2, 4> verts;

    if (!info[1].IsUndefined()) {
        const auto obj = valueAsObject(info[1]);
        if (const auto v = obj.Get("src"); !v.IsUndefined()) src = asVec4(v);
        if (const auto v = obj.Get("dst"); !v.IsUndefined()) dst = asVec4(v);
        if (const auto v = obj.Get("tint"); !v.IsUndefined()) tint = asColor(v);
        if (const auto v = obj.Get("flip"); !v.IsUndefined()) flip = static_cast<SDL_RendererFlip>(asS32(v));
        if (const auto v = obj.Get("angle"); !v.IsUndefined()) angle = asF32(v);
        if (const auto v = obj.Get("offset"); !v.IsUndefined()) offset = asVec2(v);
        if (const auto v = obj.Get("scale"); !v.IsUndefined()) scale = asVec2(v);
        if (const auto v = obj.Get("truncVtx"); !v.IsUndefined()) truncVtx = asBool(v);

        if (obj.Get("dst").IsUndefined() && !obj.Get("src").IsUndefined()) {
            dst.z = src.z;
            dst.w = src.w;
        }
    }

    dst.z *= scale.x;
    dst.w *= scale.y;

    if (angle > -0.0001f && angle < 0.0001f) {
        const auto dstX = m_origin.x + dst.x;
        const auto dstY = m_origin.y + dst.y;
        verts[0] = offset + ImVec2{ dstX, dstY };
        verts[1] = offset + ImVec2{ dstX + dst.z, dstY };
        verts[2] = offset + ImVec2{ dstX + dst.z, dstY + dst.w };
        verts[3] = offset + ImVec2{ dstX, dstY + dst.w };
    }
    else {
        const auto rcos = std::cosf(angle * 0.0174533);
        const auto rsin = std::sinf(angle * 0.0174533);
        const auto pos = offset + m_origin + ImVec2{ dst.z / 2, dst.w / 2 };
        verts[0] = pos + rotateVec2({ -dst.z * 0.5f, -dst.w * 0.5f }, rcos, rsin);
        verts[1] = pos + rotateVec2({ dst.z * 0.5f, -dst.w * 0.5f }, rcos, rsin);
        verts[2] = pos + rotateVec2({ dst.z * 0.5f, dst.w * 0.5f }, rcos, rsin);
        verts[3] = pos + rotateVec2({ -dst.z * 0.5f, dst.w * 0.5f }, rcos, rsin);
    }

    if (truncVtx) {
        for (auto& vert : verts)
            vert = truncVec2(vert);
    }

    const auto uv = buildUVs(texture->getWidth(), texture->getHeight(), src, flip);
    m_val->AddImageQuad(texture->getIdPtr(),
        verts[0], verts[1], verts[2], verts[3],
        uv[0], uv[1], uv[2], uv[3],
        ImGui::GetColorU32(tint.Value));

    return info.Env().Undefined();
}

}
