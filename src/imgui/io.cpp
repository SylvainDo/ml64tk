#include "core/instanceOf.hpp"
#include "core/opaquePointer.hpp"
#include "imgui/convert.hpp"
#include "imgui/font.hpp"
#include "imgui/fontAtlas.hpp"
#include "imgui/io.hpp"
#include "imgui/ioUserData.hpp"
#include "imgui/keyData.hpp"

#include <fmt/format.h>
#include <imgui.h>

using namespace core;

namespace imgui {

Napi::FunctionReference IO::m_ctor;

Napi::Object IO::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "IO";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&IO::getTypeId>("typeId"),
        InstanceAccessor<&IO::toDebugString>("debugString"),
        InstanceMethod<&IO::ref>("ref"),
        InstanceMethod<&IO::unref>("unref"),

        InstanceMethod<&IO::equals>("equals"),
        InstanceAccessor<&IO::getConfigFlags, &IO::setConfigFlags>("configFlags"),
        InstanceAccessor<&IO::getBackendFlags, &IO::setBackendFlags>("backendFlags"),
        InstanceAccessor<&IO::getDisplaySize, &IO::setDisplaySize>("displaySize"),
        InstanceAccessor<&IO::getDeltaTime, &IO::setDeltaTime>("deltaTime"),
        InstanceAccessor<&IO::getIniSavingRate, &IO::setIniSavingRate>("iniSavingRate"),
        InstanceAccessor<&IO::getIniFilename, &IO::setIniFilename>("iniFilename"),
        InstanceAccessor<&IO::getLogFilename, &IO::setLogFilename>("logFilename"),
        InstanceAccessor<&IO::getMouseDoubleClickTime, &IO::setMouseDoubleClickTime>("mouseDoubleClickTime"),
        InstanceAccessor<&IO::getMouseDoubleClickMaxDist, &IO::setMouseDoubleClickMaxDist>("mouseDoubleClickMaxDist"),
        InstanceAccessor<&IO::getMouseDragThreshold, &IO::setMouseDragThreshold>("mouseDragThreshold"),
        InstanceAccessor<&IO::getKeyRepeatDelay, &IO::setKeyRepeatDelay>("keyRepeatDelay"),
        InstanceAccessor<&IO::getKeyRepeatRate, &IO::setKeyRepeatRate>("keyRepeatRate"),
        InstanceAccessor<&IO::getFonts, &IO::setFonts>("fonts"),
        InstanceAccessor<&IO::getFontGlobalScale, &IO::setFontGlobalScale>("fontGlobalScale"),
        InstanceAccessor<&IO::getFontAllowUserScaling, &IO::setFontAllowUserScaling>("fontAllowUserScaling"),
        InstanceAccessor<&IO::getFontDefault, &IO::setFontDefault>("fontDefault"),
        InstanceAccessor<&IO::getDisplayFramebufferScale, &IO::setDisplayFramebufferScale>("displayFramebufferScale"),
        InstanceAccessor<&IO::getConfigDockingNoSplit, &IO::setConfigDockingNoSplit>("configDockingNoSplit"),
        InstanceAccessor<&IO::getConfigDockingWithShift, &IO::setConfigDockingWithShift>("configDockingWithShift"),
        InstanceAccessor<&IO::getConfigDockingAlwaysTabBar, &IO::setConfigDockingAlwaysTabBar>("configDockingAlwaysTabBar"),
        InstanceAccessor<&IO::getConfigDockingTransparentPayload, &IO::setConfigDockingTransparentPayload>("configDockingTransparentPayload"),
        InstanceAccessor<&IO::getConfigViewportsNoAutoMerge, &IO::setConfigViewportsNoAutoMerge>("configViewportsNoAutoMerge"),
        InstanceAccessor<&IO::getConfigViewportsNoTaskBarIcon, &IO::setConfigViewportsNoTaskBarIcon>("configViewportsNoTaskBarIcon"),
        InstanceAccessor<&IO::getConfigViewportsNoDecoration, &IO::setConfigViewportsNoDecoration>("configViewportsNoDecoration"),
        InstanceAccessor<&IO::getConfigViewportsNoDefaultParent, &IO::setConfigViewportsNoDefaultParent>("configViewportsNoDefaultParent"),
        InstanceAccessor<&IO::getMouseDrawCursor, &IO::setMouseDrawCursor>("mouseDrawCursor"),
        InstanceAccessor<&IO::getConfigMacOSXBehaviors, &IO::setConfigMacOSXBehaviors>("configMacOSXBehaviors"),
        InstanceAccessor<&IO::getConfigInputTrickleEventQueue, &IO::setConfigInputTrickleEventQueue>("configInputTrickleEventQueue"),
        InstanceAccessor<&IO::getConfigInputTextCursorBlink, &IO::setConfigInputTextCursorBlink>("configInputTextCursorBlink"),
        InstanceAccessor<&IO::getConfigInputTextEnterKeepActive, &IO::setConfigInputTextEnterKeepActive>("configInputTextEnterKeepActive"),
        InstanceAccessor<&IO::getConfigDragClickToInputText, &IO::setConfigDragClickToInputText>("configDragClickToInputText"),
        InstanceAccessor<&IO::getConfigWindowsResizeFromEdges, &IO::setConfigWindowsResizeFromEdges>("configWindowsResizeFromEdges"),
        InstanceAccessor<&IO::getConfigWindowsMoveFromTitleBarOnly, &IO::setConfigWindowsMoveFromTitleBarOnly>("configWindowsMoveFromTitleBarOnly"),
        InstanceAccessor<&IO::getConfigMemoryCompactTimer, &IO::setConfigMemoryCompactTimer>("configMemoryCompactTimer"),
        InstanceAccessor<&IO::getBackendPlatformName>("backendPlatformName"),
        InstanceAccessor<&IO::getBackendRendererName>("backendRendererName"),
        InstanceMethod<&IO::addKeyEvent>("addKeyEvent"),
        InstanceMethod<&IO::addKeyAnalogEvent>("addKeyAnalogEvent"),
        InstanceMethod<&IO::addMousePosEvent>("addMousePosEvent"),
        InstanceMethod<&IO::addMouseButtonEvent>("addMouseButtonEvent"),
        InstanceMethod<&IO::addMouseWheelEvent>("addMouseWheelEvent"),
        InstanceMethod<&IO::addMouseViewportEvent>("addMouseViewportEvent"),
        InstanceMethod<&IO::addFocusEvent>("addFocusEvent"),
        InstanceMethod<&IO::addInputCharacter>("addInputCharacter"),
        InstanceMethod<&IO::addInputCharacterUtf16>("addInputCharacterUtf16"),
        InstanceMethod<&IO::addInputCharactersUtf8>("addInputCharactersUtf8"),
        InstanceMethod<&IO::setKeyEventNativeData>("setKeyEventNativeData"),
        InstanceMethod<&IO::setAppAcceptingEvents>("setAppAcceptingEvents"),
        InstanceMethod<&IO::clearInputCharacters>("clearInputCharacters"),
        InstanceMethod<&IO::clearInputKeys>("clearInputKeys"),
        InstanceAccessor<&IO::getWantCaptureMouse, &IO::setWantCaptureMouse>("wantCaptureMouse"),
        InstanceAccessor<&IO::getWantCaptureKeyboard, &IO::setWantCaptureKeyboard>("wantCaptureKeyboard"),
        InstanceAccessor<&IO::getWantTextInput, &IO::setWantTextInput>("wantTextInput"),
        InstanceAccessor<&IO::getWantSetMousePos, &IO::setWantSetMousePos>("wantSetMousePos"),
        InstanceAccessor<&IO::getWantSaveIniSettings, &IO::setWantSaveIniSettings>("wantSaveIniSettings"),
        InstanceAccessor<&IO::getNavActive, &IO::setNavActive>("navActive"),
        InstanceAccessor<&IO::getNavVisible, &IO::setNavVisible>("navVisible"),
        InstanceAccessor<&IO::getFramerate, &IO::setFramerate>("framerate"),
        InstanceAccessor<&IO::getMetricsRenderVertices, &IO::setMetricsRenderVertices>("metricsRenderVertices"),
        InstanceAccessor<&IO::getMetricsRenderIndices, &IO::setMetricsRenderIndices>("metricsRenderIndices"),
        InstanceAccessor<&IO::getMetricsRenderWindows, &IO::setMetricsRenderWindows>("metricsRenderWindows"),
        InstanceAccessor<&IO::getMetricsActiveWindows, &IO::setMetricsActiveWindows>("metricsActiveWindows"),
        InstanceAccessor<&IO::getMetricsActiveAllocations, &IO::setMetricsActiveAllocations>("metricsActiveAllocations"),
        InstanceAccessor<&IO::getMouseDelta, &IO::setMouseDelta>("mouseDelta"),

        InstanceAccessor<&IO::internalGetMousePos, &IO::internalSetMousePos>("internalMousePos"),
        InstanceMethod<&IO::internalGetMouseDownAt>("internalGetMouseDownAt"),
        InstanceMethod<&IO::internalSetMouseDownAt>("internalSetMouseDownAt"),
        InstanceAccessor<&IO::internalGetMouseWheel, &IO::internalSetMouseWheel>("internalMouseWheel"),
        InstanceAccessor<&IO::internalGetMouseWheelH, &IO::internalSetMouseWheelH>("internalMouseWheelH"),
        InstanceAccessor<&IO::internalGetMouseHoveredViewport, &IO::internalSetMouseHoveredViewport>("internalMouseHoveredViewport"),
        InstanceAccessor<&IO::internalGetKeyCtrl, &IO::internalSetKeyCtrl>("internalKeyCtrl"),
        InstanceAccessor<&IO::internalGetKeyShift, &IO::internalSetKeyShift>("internalKeyShift"),
        InstanceAccessor<&IO::internalGetKeyAlt, &IO::internalSetKeyAlt>("internalKeyAlt"),
        InstanceAccessor<&IO::internalGetKeySuper, &IO::internalSetKeySuper>("internalKeySuper"),
        InstanceAccessor<&IO::internalGetKeyMods, &IO::internalSetKeyMods>("internalKeyMods"),
        InstanceMethod<&IO::internalGetKeyDataAt>("internalGetKeyDataAt"),
        InstanceAccessor<&IO::internalGetWantCaptureMouseUnlessPopupClose, &IO::internalSetWantCaptureMouseUnlessPopupClose>("internalWantCaptureMouseUnlessPopupClose"),
        InstanceAccessor<&IO::internalGetMousePosPrev, &IO::internalSetMousePosPrev>("internalMousePosPrev"),
        InstanceMethod<&IO::internalGetMouseClickedPosAt>("internalGetMouseClickedPosAt"),
        InstanceMethod<&IO::internalSetMouseClickedPosAt>("internalSetMouseClickedPosAt"),
        InstanceMethod<&IO::internalGetMouseClickedTimeAt>("internalGetMouseClickedTimeAt"),
        InstanceMethod<&IO::internalSetMouseClickedTimeAt>("internalSetMouseClickedTimeAt"),
        InstanceMethod<&IO::internalGetMouseClickedAt>("internalGetMouseClickedAt"),
        InstanceMethod<&IO::internalSetMouseClickedAt>("internalSetMouseClickedAt"),
        InstanceMethod<&IO::internalGetMouseDoubleClickedAt>("internalGetMouseDoubleClickedAt"),
        InstanceMethod<&IO::internalSetMouseDoubleClickedAt>("internalSetMouseDoubleClickedAt"),
        InstanceMethod<&IO::internalGetMouseClickedCountAt>("internalGetMouseClickedCountAt"),
        InstanceMethod<&IO::internalSetMouseClickedCountAt>("internalSetMouseClickedCountAt"),
        InstanceMethod<&IO::internalGetMouseClickedLastCountAt>("internalGetMouseClickedLastCountAt"),
        InstanceMethod<&IO::internalSetMouseClickedLastCountAt>("internalSetMouseClickedLastCountAt"),
        InstanceMethod<&IO::internalGetMouseReleasedAt>("internalGetMouseReleasedAt"),
        InstanceMethod<&IO::internalSetMouseReleasedAt>("internalSetMouseReleasedAt"),
        InstanceMethod<&IO::internalGetMouseDownOwnedAt>("internalGetMouseDownOwnedAt"),
        InstanceMethod<&IO::internalSetMouseDownOwnedAt>("internalSetMouseDownOwnedAt"),
        InstanceMethod<&IO::internalGetMouseDownOwnedUnlessPopupCloseAt>("internalGetMouseDownOwnedUnlessPopupCloseAt"),
        InstanceMethod<&IO::internalSetMouseDownOwnedUnlessPopupCloseAt>("internalSetMouseDownOwnedUnlessPopupCloseAt"),
        InstanceMethod<&IO::internalGetMouseDownDurationAt>("internalGetMouseDownDurationAt"),
        InstanceMethod<&IO::internalSetMouseDownDurationAt>("internalSetMouseDownDurationAt"),
        InstanceMethod<&IO::internalGetMouseDownDurationPrevAt>("internalGetMouseDownDurationPrevAt"),
        InstanceMethod<&IO::internalSetMouseDownDurationPrevAt>("internalSetMouseDownDurationPrevAt"),
        InstanceMethod<&IO::internalGetMouseDragMaxDistanceAbsAt>("internalGetMouseDragMaxDistanceAbsAt"),
        InstanceMethod<&IO::internalSetMouseDragMaxDistanceAbsAt>("internalSetMouseDragMaxDistanceAbsAt"),
        InstanceMethod<&IO::internalGetMouseDragMaxDistanceSqrAt>("internalGetMouseDragMaxDistanceSqrAt"),
        InstanceMethod<&IO::internalSetMouseDragMaxDistanceSqrAt>("internalSetMouseDragMaxDistanceSqrAt"),
        InstanceAccessor<&IO::internalGetPenPressure, &IO::internalSetPenPressure>("internalPenPressure"),
        InstanceAccessor<&IO::internalGetAppFocusLost, &IO::internalSetAppFocusLost>("internalAppFocusLost"),
        InstanceAccessor<&IO::internalGetAppAcceptingEvents, &IO::internalSetAppAcceptingEvents>("internalAppAcceptingEvents"),
        InstanceAccessor<&IO::internalGetBackendUsingLegacyKeyArrays, &IO::internalSetBackendUsingLegacyKeyArrays>("internalBackendUsingLegacyKeyArrays"),
        InstanceAccessor<&IO::internalGetBackendUsingLegacyNavInputArray, &IO::internalSetBackendUsingLegacyNavInputArray>("internalBackendUsingLegacyNavInputArray"),
        InstanceAccessor<&IO::internalGetInputQueueSurrogate, &IO::internalSetInputQueueSurrogate>("internalInputQueueSurrogate"),
        InstanceAccessor<&IO::internalGetInputQueueCharacters>("internalInputQueueCharacters")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

Napi::Object IO::create(Napi::Env, ImGuiIO* val) {
    const auto obj = m_ctor.New({});
    unwrap(obj)->m_val = val;
    return obj;
}

IO::IO(const Napi::CallbackInfo& info) : Napi::ObjectWrap<IO>{ info } {}

Napi::Value IO::getTypeId(const Napi::CallbackInfo& info) {
    return fromTypeId<IO>(info.Env());
}

Napi::Value IO::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("{}", fmt::ptr(m_val)));
}

Napi::Value IO::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value IO::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value IO::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val == IO::get(info[0]));
}

Napi::Value IO::getConfigFlags(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->ConfigFlags);
}

void IO::setConfigFlags(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigFlags = asS32(val);
}

Napi::Value IO::getBackendFlags(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->BackendFlags);
}

void IO::setBackendFlags(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->BackendFlags = asS32(val);
}

Napi::Value IO::getDisplaySize(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->DisplaySize);
}

void IO::setDisplaySize(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DisplaySize = asVec2(val);
}

Napi::Value IO::getDeltaTime(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->DeltaTime);
}

void IO::setDeltaTime(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DeltaTime = asF32(val);
}

Napi::Value IO::getIniSavingRate(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->IniSavingRate);
}

void IO::setIniSavingRate(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->IniSavingRate = asF32(val);
}

Napi::Value IO::getIniFilename(const Napi::CallbackInfo& info) {
    return !m_val->IniFilename ? info.Env().Undefined() : fromStrUtf8(info.Env(), m_val->IniFilename);
}

void IO::setIniFilename(const Napi::CallbackInfo&, const Napi::Value& val) {
    if (val.IsUndefined()) {
        m_val->IniFilename = nullptr;
    }
    else {
        auto udata = static_cast<IOUserData*>(m_val->UserData);
        udata->iniFilename = asStrUtf8(val);
        m_val->IniFilename = udata->iniFilename.c_str();
    }
}

Napi::Value IO::getLogFilename(const Napi::CallbackInfo& info) {
    return !m_val->LogFilename ? info.Env().Undefined() : fromStrUtf8(info.Env(), m_val->LogFilename);
}

void IO::setLogFilename(const Napi::CallbackInfo&, const Napi::Value& val) {
    if (val.IsUndefined()) {
        m_val->LogFilename = nullptr;
    }
    else {
        auto udata = static_cast<IOUserData*>(m_val->UserData);
        udata->logFilename = asStrUtf8(val);
        m_val->LogFilename = udata->logFilename.c_str();
    }
}

Napi::Value IO::getMouseDoubleClickTime(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->MouseDoubleClickTime);
}

void IO::setMouseDoubleClickTime(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MouseDoubleClickTime = asF32(val);
}

Napi::Value IO::getMouseDoubleClickMaxDist(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->MouseDoubleClickMaxDist);
}

void IO::setMouseDoubleClickMaxDist(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MouseDoubleClickMaxDist = asF32(val);
}

Napi::Value IO::getMouseDragThreshold(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->MouseDragThreshold);
}

void IO::setMouseDragThreshold(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MouseDragThreshold = asF32(val);
}

Napi::Value IO::getKeyRepeatDelay(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->KeyRepeatDelay);
}

void IO::setKeyRepeatDelay(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->KeyRepeatDelay = asF32(val);
}

Napi::Value IO::getKeyRepeatRate(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->KeyRepeatRate);
}

void IO::setKeyRepeatRate(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->KeyRepeatRate = asF32(val);
}

Napi::Value IO::getFonts(const Napi::CallbackInfo& info) {
    return FontAtlas::create(info.Env(), m_val->Fonts);
}

void IO::setFonts(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->Fonts = FontAtlas::get(val);
}

Napi::Value IO::getFontGlobalScale(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->FontGlobalScale);
}

void IO::setFontGlobalScale(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->FontGlobalScale = asF32(val);
}

Napi::Value IO::getFontAllowUserScaling(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->FontAllowUserScaling);
}

void IO::setFontAllowUserScaling(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->FontAllowUserScaling = asBool(val);
}

Napi::Value IO::getFontDefault(const Napi::CallbackInfo& info) {
    return Font::create(info.Env(), m_val->FontDefault);
}

void IO::setFontDefault(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->FontDefault = Font::get(val);
}

Napi::Value IO::getDisplayFramebufferScale(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->DisplayFramebufferScale);
}

void IO::setDisplayFramebufferScale(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->DisplayFramebufferScale = asVec2(val);
}

Napi::Value IO::getConfigDockingNoSplit(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigDockingNoSplit);
}

void IO::setConfigDockingNoSplit(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigDockingNoSplit = asBool(val);
}

Napi::Value IO::getConfigDockingWithShift(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigDockingWithShift);
}

void IO::setConfigDockingWithShift(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigDockingWithShift = asBool(val);
}

Napi::Value IO::getConfigDockingAlwaysTabBar(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigDockingAlwaysTabBar);
}

void IO::setConfigDockingAlwaysTabBar(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigDockingAlwaysTabBar = asBool(val);
}

Napi::Value IO::getConfigDockingTransparentPayload(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigDockingTransparentPayload);
}

void IO::setConfigDockingTransparentPayload(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigDockingTransparentPayload = asBool(val);
}

Napi::Value IO::getConfigViewportsNoAutoMerge(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigViewportsNoAutoMerge);
}

void IO::setConfigViewportsNoAutoMerge(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigViewportsNoAutoMerge = asBool(val);
}

Napi::Value IO::getConfigViewportsNoTaskBarIcon(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigViewportsNoTaskBarIcon);
}

void IO::setConfigViewportsNoTaskBarIcon(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigViewportsNoTaskBarIcon = asBool(val);
}

Napi::Value IO::getConfigViewportsNoDecoration(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigViewportsNoDecoration);
}

void IO::setConfigViewportsNoDecoration(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigViewportsNoDecoration = asBool(val);
}

Napi::Value IO::getConfigViewportsNoDefaultParent(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigViewportsNoDefaultParent);
}

void IO::setConfigViewportsNoDefaultParent(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigViewportsNoDefaultParent = asBool(val);
}

Napi::Value IO::getMouseDrawCursor(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->MouseDrawCursor);
}

void IO::setMouseDrawCursor(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MouseDrawCursor = asBool(val);
}

Napi::Value IO::getConfigMacOSXBehaviors(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigMacOSXBehaviors);
}

void IO::setConfigMacOSXBehaviors(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigMacOSXBehaviors = asBool(val);
}

Napi::Value IO::getConfigInputTrickleEventQueue(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigInputTrickleEventQueue);
}

void IO::setConfigInputTrickleEventQueue(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigInputTrickleEventQueue = asBool(val);
}

Napi::Value IO::getConfigInputTextCursorBlink(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigInputTextCursorBlink);
}

void IO::setConfigInputTextCursorBlink(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigInputTextCursorBlink = asBool(val);
}

Napi::Value IO::getConfigInputTextEnterKeepActive(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigInputTextEnterKeepActive);
}

void IO::setConfigInputTextEnterKeepActive(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigInputTextEnterKeepActive = asBool(val);
}

Napi::Value IO::getConfigDragClickToInputText(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigDragClickToInputText);
}

void IO::setConfigDragClickToInputText(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigDragClickToInputText = asBool(val);
}

Napi::Value IO::getConfigWindowsResizeFromEdges(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigWindowsResizeFromEdges);
}

void IO::setConfigWindowsResizeFromEdges(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigWindowsResizeFromEdges = asBool(val);
}

Napi::Value IO::getConfigWindowsMoveFromTitleBarOnly(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->ConfigWindowsMoveFromTitleBarOnly);
}

void IO::setConfigWindowsMoveFromTitleBarOnly(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigWindowsMoveFromTitleBarOnly = asBool(val);
}

Napi::Value IO::getConfigMemoryCompactTimer(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->ConfigMemoryCompactTimer);
}

void IO::setConfigMemoryCompactTimer(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->ConfigMemoryCompactTimer = asF32(val);
}

Napi::Value IO::getBackendPlatformName(const Napi::CallbackInfo& info) {
    return !m_val->BackendPlatformName ? info.Env().Undefined() : fromStrUtf8(info.Env(), m_val->BackendPlatformName);
}

Napi::Value IO::getBackendRendererName(const Napi::CallbackInfo& info) {
    return !m_val->BackendRendererName ? info.Env().Undefined() : fromStrUtf8(info.Env(), m_val->BackendRendererName);
}

Napi::Value IO::addKeyEvent(const Napi::CallbackInfo& info) {
    m_val->AddKeyEvent(
        /* key */ asS32(info[0]),
        /* down */ asBool(info[1]));
    return info.Env().Undefined();
}

Napi::Value IO::addKeyAnalogEvent(const Napi::CallbackInfo& info) {
    m_val->AddKeyAnalogEvent(
        /* key */ asS32(info[0]),
        /* down */ asBool(info[1]),
        /* v */ asF32(info[2]));
    return info.Env().Undefined();
}

Napi::Value IO::addMousePosEvent(const Napi::CallbackInfo& info) {
    m_val->AddMousePosEvent(
        /* x */ asF32(info[0]),
        /* y */ asF32(info[1]));
    return info.Env().Undefined();
}

Napi::Value IO::addMouseButtonEvent(const Napi::CallbackInfo& info) {
    m_val->AddMouseButtonEvent(
        /* button */ asS32(info[0]),
        /* down */ asBool(info[1]));
    return info.Env().Undefined();
}

Napi::Value IO::addMouseWheelEvent(const Napi::CallbackInfo& info) {
    m_val->AddMouseWheelEvent(
        /* wh_x */ asF32(info[0]),
        /* wh_y */ asF32(info[1]));
    return info.Env().Undefined();
}

Napi::Value IO::addMouseViewportEvent(const Napi::CallbackInfo& info) {
    m_val->AddMouseViewportEvent(/* id */ asU32(info[0]));
    return info.Env().Undefined();
}

Napi::Value IO::addFocusEvent(const Napi::CallbackInfo& info) {
    m_val->AddFocusEvent(/* focused */ asBool(info[0]));
    return info.Env().Undefined();
}

Napi::Value IO::addInputCharacter(const Napi::CallbackInfo& info) {
    m_val->AddInputCharacter(/* c */ asU32(info[0]));
    return info.Env().Undefined();
}

Napi::Value IO::addInputCharacterUtf16(const Napi::CallbackInfo& info) {
    m_val->AddInputCharacterUTF16(/* c */ asU16(info[0]));
    return info.Env().Undefined();
}

Napi::Value IO::addInputCharactersUtf8(const Napi::CallbackInfo& info) {
    m_val->AddInputCharactersUTF8(/* str */ asStrUtf8(info[0]).c_str());
    return info.Env().Undefined();
}

Napi::Value IO::setKeyEventNativeData(const Napi::CallbackInfo& info) {
    m_val->SetKeyEventNativeData(
        /* key */ asS32(info[0]),
        /* native_keycode */ asS32(info[1]),
        /* native_scancode */ asS32(info[2]));
    return info.Env().Undefined();
}

Napi::Value IO::setAppAcceptingEvents(const Napi::CallbackInfo& info) {
    m_val->SetAppAcceptingEvents(/* accepting_events */ asBool(info[0]));
    return info.Env().Undefined();
}

Napi::Value IO::clearInputCharacters(const Napi::CallbackInfo& info) {
    m_val->ClearInputCharacters();
    return info.Env().Undefined();
}

Napi::Value IO::clearInputKeys(const Napi::CallbackInfo& info) {
    m_val->ClearInputKeys();
    return info.Env().Undefined();
}

Napi::Value IO::getWantCaptureMouse(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->WantCaptureMouse);
}

void IO::setWantCaptureMouse(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WantCaptureMouse = asBool(val);
}

Napi::Value IO::getWantCaptureKeyboard(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->WantCaptureKeyboard);
}

void IO::setWantCaptureKeyboard(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WantCaptureKeyboard = asBool(val);
}

Napi::Value IO::getWantTextInput(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->WantTextInput);
}

void IO::setWantTextInput(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WantTextInput = asBool(val);
}

Napi::Value IO::getWantSetMousePos(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->WantSetMousePos);
}

void IO::setWantSetMousePos(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WantSetMousePos = asBool(val);
}

Napi::Value IO::getWantSaveIniSettings(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->WantSaveIniSettings);
}

void IO::setWantSaveIniSettings(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WantSaveIniSettings = asBool(val);
}

Napi::Value IO::getNavActive(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->NavActive);
}

void IO::setNavActive(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->NavActive = asBool(val);
}

Napi::Value IO::getNavVisible(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->NavVisible);
}

void IO::setNavVisible(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->NavVisible = asBool(val);
}

Napi::Value IO::getFramerate(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->Framerate);
}

void IO::setFramerate(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->Framerate = asF32(val);
}

Napi::Value IO::getMetricsRenderVertices(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->MetricsRenderVertices);
}

void IO::setMetricsRenderVertices(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MetricsRenderVertices = asS32(val);
}

Napi::Value IO::getMetricsRenderIndices(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->MetricsRenderIndices);
}

void IO::setMetricsRenderIndices(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MetricsRenderIndices = asS32(val);
}

Napi::Value IO::getMetricsRenderWindows(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->MetricsRenderWindows);
}

void IO::setMetricsRenderWindows(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MetricsRenderWindows = asS32(val);
}

Napi::Value IO::getMetricsActiveWindows(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->MetricsActiveWindows);
}

void IO::setMetricsActiveWindows(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MetricsActiveWindows = asS32(val);
}

Napi::Value IO::getMetricsActiveAllocations(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->MetricsActiveAllocations);
}

void IO::setMetricsActiveAllocations(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MetricsActiveAllocations = asS32(val);
}

Napi::Value IO::getMouseDelta(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->MouseDelta);
}

void IO::setMouseDelta(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MouseDelta = asVec2(val);
}

Napi::Value IO::internalGetMousePos(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->MousePos);
}

void IO::internalSetMousePos(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MousePos = asVec2(val);
}

Napi::Value IO::internalGetMouseDownAt(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->MouseDown[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseDownAt(const Napi::CallbackInfo& info) {
    m_val->MouseDown[asS32(info[0])] = asBool(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseWheel(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->MouseWheel);
}

void IO::internalSetMouseWheel(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MouseWheel = asF32(val);
}

Napi::Value IO::internalGetMouseWheelH(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->MouseWheelH);
}

void IO::internalSetMouseWheelH(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MouseWheelH = asF32(val);
}

Napi::Value IO::internalGetMouseHoveredViewport(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), m_val->MouseHoveredViewport);
}

void IO::internalSetMouseHoveredViewport(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MouseHoveredViewport = asU32(val);
}

Napi::Value IO::internalGetKeyCtrl(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->KeyCtrl);
}

void IO::internalSetKeyCtrl(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->KeyCtrl = asBool(val);
}

Napi::Value IO::internalGetKeyShift(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->KeyShift);
}

void IO::internalSetKeyShift(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->KeyShift = asBool(val);
}

Napi::Value IO::internalGetKeyAlt(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->KeyAlt);
}

void IO::internalSetKeyAlt(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->KeyAlt = asBool(val);
}

Napi::Value IO::internalGetKeySuper(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->KeySuper);
}

void IO::internalSetKeySuper(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->KeySuper = asBool(val);
}

Napi::Value IO::internalGetKeyMods(const Napi::CallbackInfo& info) {
    return fromS32(info.Env(), m_val->KeyMods);
}

void IO::internalSetKeyMods(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->KeyMods = asS32(val);
}

Napi::Value IO::internalGetKeyDataAt(const Napi::CallbackInfo& info) {
    return KeyData::create(info.Env(), &m_val->KeysData[asS32(info[0]) - ImGuiKey_NamedKey_BEGIN]);
}

Napi::Value IO::internalGetWantCaptureMouseUnlessPopupClose(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->WantCaptureMouseUnlessPopupClose);
}

void IO::internalSetWantCaptureMouseUnlessPopupClose(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->WantCaptureMouseUnlessPopupClose = asBool(val);
}

Napi::Value IO::internalGetMousePosPrev(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->MousePosPrev);
}

void IO::internalSetMousePosPrev(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->MousePosPrev = asVec2(val);
}

Napi::Value IO::internalGetMouseClickedPosAt(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->MouseClickedPos[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseClickedPosAt(const Napi::CallbackInfo& info) {
    m_val->MouseClickedPos[asS32(info[0])] = asVec2(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseClickedTimeAt(const Napi::CallbackInfo& info) {
    return fromF64(info.Env(), m_val->MouseClickedTime[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseClickedTimeAt(const Napi::CallbackInfo& info) {
    m_val->MouseClickedTime[asS32(info[0])] = asF64(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseClickedAt(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->MouseClicked[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseClickedAt(const Napi::CallbackInfo& info) {
    m_val->MouseClicked[asS32(info[0])] = asBool(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseDoubleClickedAt(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->MouseDoubleClicked[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseDoubleClickedAt(const Napi::CallbackInfo& info) {
    m_val->MouseDoubleClicked[asS32(info[0])] = asBool(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseClickedCountAt(const Napi::CallbackInfo& info) {
    return fromU16(info.Env(), m_val->MouseClickedCount[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseClickedCountAt(const Napi::CallbackInfo& info) {
    m_val->MouseClickedCount[asS32(info[0])] = asU16(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseClickedLastCountAt(const Napi::CallbackInfo& info) {
    return fromU16(info.Env(), m_val->MouseClickedLastCount[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseClickedLastCountAt(const Napi::CallbackInfo& info) {
    m_val->MouseClickedLastCount[asS32(info[0])] = asU16(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseReleasedAt(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->MouseReleased[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseReleasedAt(const Napi::CallbackInfo& info) {
    m_val->MouseReleased[asS32(info[0])] = asBool(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseDownOwnedAt(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->MouseDownOwned[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseDownOwnedAt(const Napi::CallbackInfo& info) {
    m_val->MouseDownOwned[asS32(info[0])] = asBool(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseDownOwnedUnlessPopupCloseAt(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->MouseDownOwnedUnlessPopupClose[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseDownOwnedUnlessPopupCloseAt(const Napi::CallbackInfo& info) {
    m_val->MouseDownOwnedUnlessPopupClose[asS32(info[0])] = asBool(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseDownDurationAt(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->MouseDownDuration[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseDownDurationAt(const Napi::CallbackInfo& info) {
    m_val->MouseDownDuration[asS32(info[0])] = asF32(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseDownDurationPrevAt(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->MouseDownDurationPrev[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseDownDurationPrevAt(const Napi::CallbackInfo& info) {
    m_val->MouseDownDurationPrev[asS32(info[0])] = asF32(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseDragMaxDistanceAbsAt(const Napi::CallbackInfo& info) {
    return fromVec2(info.Env(), m_val->MouseDragMaxDistanceAbs[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseDragMaxDistanceAbsAt(const Napi::CallbackInfo& info) {
    m_val->MouseDragMaxDistanceAbs[asS32(info[0])] = asVec2(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetMouseDragMaxDistanceSqrAt(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->MouseDragMaxDistanceSqr[asS32(info[0])]);
}

Napi::Value IO::internalSetMouseDragMaxDistanceSqrAt(const Napi::CallbackInfo& info) {
    m_val->MouseDragMaxDistanceSqr[asS32(info[0])] = asF32(info[1]);
    return info.Env().Undefined();
}

Napi::Value IO::internalGetPenPressure(const Napi::CallbackInfo& info) {
    return fromF32(info.Env(), m_val->PenPressure);
}

void IO::internalSetPenPressure(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->PenPressure = asF32(val);
}

Napi::Value IO::internalGetAppFocusLost(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->AppFocusLost);
}

void IO::internalSetAppFocusLost(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->AppFocusLost = asBool(val);
}

Napi::Value IO::internalGetAppAcceptingEvents(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->AppAcceptingEvents);
}

void IO::internalSetAppAcceptingEvents(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->AppAcceptingEvents = asBool(val);
}

Napi::Value IO::internalGetBackendUsingLegacyKeyArrays(const Napi::CallbackInfo& info) {
    return fromS8(info.Env(), m_val->BackendUsingLegacyKeyArrays);
}

void IO::internalSetBackendUsingLegacyKeyArrays(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->BackendUsingLegacyKeyArrays = asS8(val);
}

Napi::Value IO::internalGetBackendUsingLegacyNavInputArray(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), m_val->BackendUsingLegacyNavInputArray);
}

void IO::internalSetBackendUsingLegacyNavInputArray(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->BackendUsingLegacyNavInputArray = asBool(val);
}

Napi::Value IO::internalGetInputQueueSurrogate(const Napi::CallbackInfo& info) {
    return fromU16(info.Env(), m_val->InputQueueSurrogate);
}

void IO::internalSetInputQueueSurrogate(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_val->InputQueueSurrogate = asU16(val);
}

Napi::Value IO::internalGetInputQueueCharacters(const Napi::CallbackInfo& info) {
    return Napi::ArrayBuffer::New(info.Env(), m_val->InputQueueCharacters.Data, sizeof(ImWchar) * m_val->InputQueueCharacters.Size);
}

}
