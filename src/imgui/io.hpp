#pragma once

#include <napi.h>

struct ImGuiIO;

namespace imgui {

class IO final : public Napi::ObjectWrap<IO> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);
    static Napi::Object create(Napi::Env env, ImGuiIO* val);

    static IO* unwrap(const Napi::Object& obj) {
        return IO::Unwrap(obj);
    }

    static IO* unwrap(const Napi::Value& val) {
        return IO::Unwrap(val.As<Napi::Object>());
    }

    template <class T>
    static ImGuiIO* get(const T& val) {
        return unwrap(val)->m_val;
    }

    ImGuiIO* get() {
        return m_val;
    }

    IO(const Napi::CallbackInfo& info);
    virtual ~IO() = default;

private:
    static Napi::FunctionReference m_ctor;
    ImGuiIO* m_val;

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getConfigFlags(const Napi::CallbackInfo& info);
    void setConfigFlags(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getBackendFlags(const Napi::CallbackInfo& info);
    void setBackendFlags(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDisplaySize(const Napi::CallbackInfo& info);
    void setDisplaySize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDeltaTime(const Napi::CallbackInfo& info);
    void setDeltaTime(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getIniSavingRate(const Napi::CallbackInfo& info);
    void setIniSavingRate(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getIniFilename(const Napi::CallbackInfo& info);
    void setIniFilename(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getLogFilename(const Napi::CallbackInfo& info);
    void setLogFilename(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMouseDoubleClickTime(const Napi::CallbackInfo& info);
    void setMouseDoubleClickTime(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMouseDoubleClickMaxDist(const Napi::CallbackInfo& info);
    void setMouseDoubleClickMaxDist(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMouseDragThreshold(const Napi::CallbackInfo& info);
    void setMouseDragThreshold(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getKeyRepeatDelay(const Napi::CallbackInfo& info);
    void setKeyRepeatDelay(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getKeyRepeatRate(const Napi::CallbackInfo& info);
    void setKeyRepeatRate(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFonts(const Napi::CallbackInfo& info);
    void setFonts(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFontGlobalScale(const Napi::CallbackInfo& info);
    void setFontGlobalScale(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFontAllowUserScaling(const Napi::CallbackInfo& info);
    void setFontAllowUserScaling(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFontDefault(const Napi::CallbackInfo& info);
    void setFontDefault(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getDisplayFramebufferScale(const Napi::CallbackInfo& info);
    void setDisplayFramebufferScale(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigDockingNoSplit(const Napi::CallbackInfo& info);
    void setConfigDockingNoSplit(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigDockingWithShift(const Napi::CallbackInfo& info);
    void setConfigDockingWithShift(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigDockingAlwaysTabBar(const Napi::CallbackInfo& info);
    void setConfigDockingAlwaysTabBar(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigDockingTransparentPayload(const Napi::CallbackInfo& info);
    void setConfigDockingTransparentPayload(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigViewportsNoAutoMerge(const Napi::CallbackInfo& info);
    void setConfigViewportsNoAutoMerge(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigViewportsNoTaskBarIcon(const Napi::CallbackInfo& info);
    void setConfigViewportsNoTaskBarIcon(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigViewportsNoDecoration(const Napi::CallbackInfo& info);
    void setConfigViewportsNoDecoration(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigViewportsNoDefaultParent(const Napi::CallbackInfo& info);
    void setConfigViewportsNoDefaultParent(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMouseDrawCursor(const Napi::CallbackInfo& info);
    void setMouseDrawCursor(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigMacOSXBehaviors(const Napi::CallbackInfo& info);
    void setConfigMacOSXBehaviors(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigInputTrickleEventQueue(const Napi::CallbackInfo& info);
    void setConfigInputTrickleEventQueue(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigInputTextCursorBlink(const Napi::CallbackInfo& info);
    void setConfigInputTextCursorBlink(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigInputTextEnterKeepActive(const Napi::CallbackInfo& info);
    void setConfigInputTextEnterKeepActive(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigDragClickToInputText(const Napi::CallbackInfo& info);
    void setConfigDragClickToInputText(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigWindowsResizeFromEdges(const Napi::CallbackInfo& info);
    void setConfigWindowsResizeFromEdges(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigWindowsMoveFromTitleBarOnly(const Napi::CallbackInfo& info);
    void setConfigWindowsMoveFromTitleBarOnly(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getConfigMemoryCompactTimer(const Napi::CallbackInfo& info);
    void setConfigMemoryCompactTimer(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getBackendPlatformName(const Napi::CallbackInfo& info);
    Napi::Value getBackendRendererName(const Napi::CallbackInfo& info);
    Napi::Value addKeyEvent(const Napi::CallbackInfo& info);
    Napi::Value addKeyAnalogEvent(const Napi::CallbackInfo& info);
    Napi::Value addMousePosEvent(const Napi::CallbackInfo& info);
    Napi::Value addMouseButtonEvent(const Napi::CallbackInfo& info);
    Napi::Value addMouseWheelEvent(const Napi::CallbackInfo& info);
    Napi::Value addMouseViewportEvent(const Napi::CallbackInfo& info);
    Napi::Value addFocusEvent(const Napi::CallbackInfo& info);
    Napi::Value addInputCharacter(const Napi::CallbackInfo& info);
    Napi::Value addInputCharacterUtf16(const Napi::CallbackInfo& info);
    Napi::Value addInputCharactersUtf8(const Napi::CallbackInfo& info);
    Napi::Value setKeyEventNativeData(const Napi::CallbackInfo& info);
    Napi::Value setAppAcceptingEvents(const Napi::CallbackInfo& info);
    Napi::Value clearInputCharacters(const Napi::CallbackInfo& info);
    Napi::Value clearInputKeys(const Napi::CallbackInfo& info);
    Napi::Value getWantCaptureMouse(const Napi::CallbackInfo& info);
    void setWantCaptureMouse(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWantCaptureKeyboard(const Napi::CallbackInfo& info);
    void setWantCaptureKeyboard(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWantTextInput(const Napi::CallbackInfo& info);
    void setWantTextInput(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWantSetMousePos(const Napi::CallbackInfo& info);
    void setWantSetMousePos(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getWantSaveIniSettings(const Napi::CallbackInfo& info);
    void setWantSaveIniSettings(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getNavActive(const Napi::CallbackInfo& info);
    void setNavActive(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getNavVisible(const Napi::CallbackInfo& info);
    void setNavVisible(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getFramerate(const Napi::CallbackInfo& info);
    void setFramerate(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMetricsRenderVertices(const Napi::CallbackInfo& info);
    void setMetricsRenderVertices(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMetricsRenderIndices(const Napi::CallbackInfo& info);
    void setMetricsRenderIndices(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMetricsRenderWindows(const Napi::CallbackInfo& info);
    void setMetricsRenderWindows(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMetricsActiveWindows(const Napi::CallbackInfo& info);
    void setMetricsActiveWindows(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMetricsActiveAllocations(const Napi::CallbackInfo& info);
    void setMetricsActiveAllocations(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMouseDelta(const Napi::CallbackInfo& info);
    void setMouseDelta(const Napi::CallbackInfo&, const Napi::Value& val);

    Napi::Value internalGetMousePos(const Napi::CallbackInfo& info);
    void internalSetMousePos(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetMouseDownAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseDownAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseWheel(const Napi::CallbackInfo& info);
    void internalSetMouseWheel(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetMouseWheelH(const Napi::CallbackInfo& info);
    void internalSetMouseWheelH(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetMouseHoveredViewport(const Napi::CallbackInfo& info);
    void internalSetMouseHoveredViewport(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetKeyCtrl(const Napi::CallbackInfo& info);
    void internalSetKeyCtrl(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetKeyShift(const Napi::CallbackInfo& info);
    void internalSetKeyShift(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetKeyAlt(const Napi::CallbackInfo& info);
    void internalSetKeyAlt(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetKeySuper(const Napi::CallbackInfo& info);
    void internalSetKeySuper(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetKeyMods(const Napi::CallbackInfo& info);
    void internalSetKeyMods(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetKeyDataAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetWantCaptureMouseUnlessPopupClose(const Napi::CallbackInfo& info);
    void internalSetWantCaptureMouseUnlessPopupClose(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetMousePosPrev(const Napi::CallbackInfo& info);
    void internalSetMousePosPrev(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetMouseClickedPosAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseClickedPosAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseClickedTimeAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseClickedTimeAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseClickedAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseClickedAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseDoubleClickedAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseDoubleClickedAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseClickedCountAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseClickedCountAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseClickedLastCountAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseClickedLastCountAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseReleasedAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseReleasedAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseDownOwnedAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseDownOwnedAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseDownOwnedUnlessPopupCloseAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseDownOwnedUnlessPopupCloseAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseDownDurationAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseDownDurationAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseDownDurationPrevAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseDownDurationPrevAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseDragMaxDistanceAbsAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseDragMaxDistanceAbsAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetMouseDragMaxDistanceSqrAt(const Napi::CallbackInfo& info);
    Napi::Value internalSetMouseDragMaxDistanceSqrAt(const Napi::CallbackInfo& info);
    Napi::Value internalGetPenPressure(const Napi::CallbackInfo& info);
    void internalSetPenPressure(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetAppFocusLost(const Napi::CallbackInfo& info);
    void internalSetAppFocusLost(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetAppAcceptingEvents(const Napi::CallbackInfo& info);
    void internalSetAppAcceptingEvents(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetBackendUsingLegacyKeyArrays(const Napi::CallbackInfo& info);
    void internalSetBackendUsingLegacyKeyArrays(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetBackendUsingLegacyNavInputArray(const Napi::CallbackInfo& info);
    void internalSetBackendUsingLegacyNavInputArray(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetInputQueueSurrogate(const Napi::CallbackInfo& info);
    void internalSetInputQueueSurrogate(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value internalGetInputQueueCharacters(const Napi::CallbackInfo& info);
};

}
