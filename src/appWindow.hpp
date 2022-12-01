#pragma once

#include "imgui/ioUserData.hpp"

#include <array>
#include <memory>
#include <thread>

#include <imgui.h>
#include <napi.h>
#include <SDL_video.h>

union SDL_Event;

class AppWindow final : public Napi::ObjectWrap<AppWindow> {
public:
    static Napi::Object initialize(Napi::Env env, Napi::Object exports);

    static AppWindow* unwrap(const Napi::Object& obj) {
        return AppWindow::Unwrap(obj);
    }

    static AppWindow* unwrap(const Napi::Value& val) {
        return AppWindow::Unwrap(val.As<Napi::Object>());
    }

    static AppWindow* getInstance() {
        return m_instance;
    }

    AppWindow(const Napi::CallbackInfo& info);
    virtual ~AppWindow();

private:
    enum Callback {
        Init,
        BeforeNewFrame,
        NewFrame,
        BeforeRender,
        Render,
        DropFile,
        Close,
        NumCallbacks
    };

    static Napi::FunctionReference m_ctor;
    static AppWindow* m_instance;
    std::thread::id m_threadId;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window{ nullptr, SDL_DestroyWindow };
    std::unique_ptr<void, decltype(&SDL_GL_DeleteContext)> m_glContext{ nullptr, SDL_GL_DeleteContext };
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> m_icon{ nullptr, SDL_FreeSurface };
    ImGuiContext* m_imguiContext;
    imgui::IOUserData m_ioUserData;
    std::array<std::unique_ptr<Napi::FunctionReference>, Callback::NumCallbacks> m_callbacks;
    ImColor m_clearColor{ 0.45f, 0.55f, 0.6f, 0.0f };
    bool m_inited{}, m_done{};

    void doStep(Napi::Env env);
    void doFrame();
    void call(Callback callback, const std::initializer_list<napi_value>& args = {});
    static int eventWatch(void* userdata, SDL_Event* event);
    void closeReal();

    static Napi::Value getTicks(const Napi::CallbackInfo& info);

    Napi::Value getTypeId(const Napi::CallbackInfo& info);
    Napi::Value toDebugString(const Napi::CallbackInfo& info);
    Napi::Value ref(const Napi::CallbackInfo& info);
    Napi::Value unref(const Napi::CallbackInfo& info);

    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value on(const Napi::CallbackInfo& info);
    Napi::Value doIteration(const Napi::CallbackInfo& info);
    Napi::Value close(const Napi::CallbackInfo& info);
    Napi::Value getTitle(const Napi::CallbackInfo& info);
    void setTitle(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value setIconFromFile(const Napi::CallbackInfo& info);
    Napi::Value setIconFromMemory(const Napi::CallbackInfo& info);
    Napi::Value getSize(const Napi::CallbackInfo& info);
    void setSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMinimumSize(const Napi::CallbackInfo& info);
    void setMinimumSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getMaximumSize(const Napi::CallbackInfo& info);
    void setMaximumSize(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value getClearColor(const Napi::CallbackInfo& info);
    void setClearColor(const Napi::CallbackInfo&, const Napi::Value& val);
    Napi::Value isVisible(const Napi::CallbackInfo& info);
    Napi::Value hide(const Napi::CallbackInfo& info);
    Napi::Value show(const Napi::CallbackInfo& info);
};
