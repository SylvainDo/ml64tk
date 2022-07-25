#include "core/type/instanceOf.hpp"
#include "imgui/convert.hpp"
#include "imgui/viewport.hpp"
#include "appWindow.hpp"

#include <fmt/format.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <SDL_image.h>

using namespace core::type::convert;

Napi::FunctionReference AppWindow::m_ctor;
AppWindow* AppWindow::m_instance;

Napi::Object AppWindow::initialize(Napi::Env env, Napi::Object exports) {
    constexpr const char* className = "AppWindow";
    const auto class_ = DefineClass(env, className, {
        InstanceAccessor<&AppWindow::getTypeId>("typeId"),
        InstanceAccessor<&AppWindow::toDebugString>("debugString"),
        InstanceMethod<&AppWindow::ref>("ref"),
        InstanceMethod<&AppWindow::unref>("unref"),

        InstanceMethod<&AppWindow::equals>("equals"),
        InstanceMethod<&AppWindow::on>("on"),
        InstanceMethod<&AppWindow::doIteration>("doIteration"),
        InstanceAccessor<&AppWindow::getTitle, &AppWindow::setTitle>("title"),
        InstanceMethod<&AppWindow::setIconFromFile>("setIconFromFile"),
        InstanceMethod<&AppWindow::setIconFromMemory>("setIconFromMemory"),
        InstanceAccessor<&AppWindow::getSize, &AppWindow::setSize>("size"),
        InstanceAccessor<&AppWindow::getMinimumSize, &AppWindow::setMinimumSize>("minimumSize"),
        InstanceAccessor<&AppWindow::getMaximumSize, &AppWindow::setMaximumSize>("maximumSize"),
        InstanceAccessor<&AppWindow::getClearColor, &AppWindow::setClearColor>("clearColor")
    });

    m_ctor = Napi::Persistent(class_);
    m_ctor.SuppressDestruct();

    exports.Set(className, class_);
    return exports;
}

AppWindow::AppWindow(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<AppWindow>{ info },
    m_window{ nullptr, SDL_DestroyWindow },
    m_glContext{ nullptr, SDL_GL_DeleteContext },
    m_icon{ nullptr, SDL_FreeSurface } {

    if (m_instance)
        throw Napi::Error::New(info.Env(), "only a single AppWindow instance is supported");
    m_instance = this;
    m_threadId = std::this_thread::get_id();

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS , 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES , 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    m_window.reset(SDL_CreateWindow("AppWindow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN));
    if (!m_window)
        throw Napi::Error::New(info.Env(), fmt::format("failed to create sdl2 window: {}", SDL_GetError()));
    m_glContext.reset(SDL_GL_CreateContext(m_window.get()));
    if (!m_glContext)
        throw Napi::Error::New(info.Env(), fmt::format("failed to create gl context: {}", SDL_GetError()));
    SDL_GL_MakeCurrent(m_window.get(), m_glContext.get());
    SDL_GL_SetSwapInterval(0);
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
        throw Napi::Error::New(info.Env(), "failed to create gl context");

    IMGUI_CHECKVERSION();
    m_imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(m_imguiContext);
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    auto& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    io.UserData = &m_ioUserData;
    if (!ImGui_ImplSDL2_InitForOpenGL(m_window.get(), m_glContext.get()))
        throw Napi::Error::New(info.Env(), "failed to initialize imgui impl sdl2");
    if (!ImGui_ImplOpenGL3_Init("#version 330"))
        throw Napi::Error::New(info.Env(), "failed to initialize imgui impl opengl3");
}

AppWindow::~AppWindow() {
#ifdef _WIN32
    SDL_DelEventWatch(eventWatch, this);
#endif
    ImGui::SetCurrentContext(m_imguiContext);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void AppWindow::doStep(Napi::Env env) {
    SDL_GL_MakeCurrent(m_window.get(), m_glContext.get());
    ImGui::SetCurrentContext(m_imguiContext);

    if (!m_inited) {
        call(Callback::Init);
        SDL_SetWindowPosition(m_window.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(m_window.get());
        SDL_FlashWindow(m_window.get(), SDL_FLASH_UNTIL_FOCUSED);
#ifdef _WIN32
        SDL_AddEventWatch(eventWatch, this);
#endif
        m_inited = true;
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);
        if (e.type == SDL_QUIT
            || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE
                && e.window.windowID == SDL_GetWindowID(m_window.get()))) {
            SDL_HideWindow(m_window.get());
            m_done = true;
        }
        else if (e.type == SDL_DROPFILE) {
            const std::unique_ptr<char, decltype(&SDL_free)> file{ e.drop.file, SDL_free };
            call(Callback::DropFile, {
                fromStrUtf8(env, file.get()),
                imgui::Viewport::create(env, ImGui::FindViewportByPlatformHandle(SDL_GetWindowFromID(e.drop.windowID)))
            });
        }
    }

    doFrame();
}

void AppWindow::doFrame() {
    call(Callback::BeforeNewFrame);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    call(Callback::NewFrame);
    ImGui::Render();

    const auto& io = ImGui::GetIO();
    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    glClearColor(m_clearColor.Value.x, m_clearColor.Value.y, m_clearColor.Value.z, m_clearColor.Value.w);
    glClear(GL_COLOR_BUFFER_BIT);
    call(Callback::BeforeRender);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    call(Callback::Render);

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        auto backupCurrentWindow = SDL_GL_GetCurrentWindow();
        auto backupCurrentContext = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
    }

    SDL_GL_SwapWindow(m_window.get());
}

void AppWindow::call(Callback callback, const std::initializer_list<napi_value>& args) {
    auto& func = m_callbacks[callback];
    if (func && !func->IsEmpty()) func->Call(args);
}

int AppWindow::eventWatch(void* userdata, SDL_Event* event) {
    auto thisx = static_cast<AppWindow*>(userdata);
    if (std::this_thread::get_id() == thisx->m_threadId && event->type == SDL_WINDOWEVENT
        && (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event->window.event == SDL_WINDOWEVENT_MOVED))
        thisx->doFrame();
    return 0;
}

Napi::Value AppWindow::getTypeId(const Napi::CallbackInfo& info) {
    return core::type::fromTypeId<AppWindow>(info.Env());
}

Napi::Value AppWindow::toDebugString(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), fmt::format("null"));
}

Napi::Value AppWindow::ref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Ref());
}

Napi::Value AppWindow::unref(const Napi::CallbackInfo& info) {
    return fromU32(info.Env(), Unref());
}

Napi::Value AppWindow::equals(const Napi::CallbackInfo& info) {
    return fromBool(info.Env(), this == AppWindow::unwrap(info[0]));
}

Napi::Value AppWindow::on(const Napi::CallbackInfo& info) {
    const auto name = asStrUtf8(info[0]);
    std::unique_ptr<Napi::FunctionReference>* callback;
    if (name == "init") callback = &m_callbacks[Callback::Init];
    else if (name == "before-new-frame") callback = &m_callbacks[Callback::BeforeNewFrame];
    else if (name == "new-frame") callback = &m_callbacks[Callback::NewFrame];
    else if (name == "before-render") callback = &m_callbacks[Callback::BeforeRender];
    else if (name == "render") callback = &m_callbacks[Callback::Render];
    else if (name == "drop-file") callback = &m_callbacks[Callback::DropFile];
    else throw Napi::Error::New(info.Env(), "invalid callback name");
    *callback = std::make_unique<Napi::FunctionReference>();
    if (!info[1].IsUndefined())
        *callback->get() = Napi::Persistent(valueAsFunction(info[1]));
    return info.Env().Undefined();
}

Napi::Value AppWindow::doIteration(const Napi::CallbackInfo& info) {
    if (!m_done) doStep(info.Env());
    return fromBool(info.Env(), m_done);
}

Napi::Value AppWindow::getTitle(const Napi::CallbackInfo& info) {
    return fromStrUtf8(info.Env(), SDL_GetWindowTitle(m_window.get()));
}

void AppWindow::setTitle(const Napi::CallbackInfo&, const Napi::Value& val) {
    SDL_SetWindowTitle(m_window.get(), asStrUtf8(val).c_str());
}

Napi::Value AppWindow::setIconFromFile(const Napi::CallbackInfo& info) {
    const auto filename = asStrUtf8(info[0]);
    m_icon.reset(IMG_Load(filename.c_str()));
    if (!m_icon) throw Napi::Error::New(info.Env(), fmt::format("failed to load icon from file `{}`: {}", filename, IMG_GetError()));
    SDL_SetWindowIcon(m_window.get(), m_icon.get());
    return info.Env().Undefined();
}

Napi::Value AppWindow::setIconFromMemory(const Napi::CallbackInfo& info) {
    auto data = info[0].As<Napi::ArrayBuffer>();
    m_icon.reset(IMG_Load_RW(SDL_RWFromMem(data.Data(), data.ByteLength()), true));
    if (!m_icon) throw Napi::Error::New(info.Env(), fmt::format("failed to load icon from memory: {}", IMG_GetError()));
    SDL_SetWindowIcon(m_window.get(), m_icon.get());
    return info.Env().Undefined();
}

Napi::Value AppWindow::getSize(const Napi::CallbackInfo& info) {
    int w, h;
    SDL_GetWindowSize(m_window.get(), &w, &h);
    return imgui::fromVec2(info.Env(), w, h);
}

void AppWindow::setSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    const auto size = imgui::asVec2(val);
    SDL_SetWindowSize(m_window.get(), static_cast<int>(size.x), static_cast<int>(size.y));
}

Napi::Value AppWindow::getMinimumSize(const Napi::CallbackInfo& info) {
    int w, h;
    SDL_GetWindowMinimumSize(m_window.get(), &w, &h);
    return imgui::fromVec2(info.Env(), w, h);
}

void AppWindow::setMinimumSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    const auto size = imgui::asVec2(val);
    SDL_SetWindowMinimumSize(m_window.get(), static_cast<int>(size.x), static_cast<int>(size.y));
}

Napi::Value AppWindow::getMaximumSize(const Napi::CallbackInfo& info) {
    int w, h;
    SDL_GetWindowMaximumSize(m_window.get(), &w, &h);
    return imgui::fromVec2(info.Env(), w, h);
}

void AppWindow::setMaximumSize(const Napi::CallbackInfo&, const Napi::Value& val) {
    const auto size = imgui::asVec2(val);
    SDL_SetWindowMaximumSize(m_window.get(), static_cast<int>(size.x), static_cast<int>(size.y));
}

Napi::Value AppWindow::getClearColor(const Napi::CallbackInfo& info) {
    return imgui::fromColor(info.Env(), m_clearColor);
}

void AppWindow::setClearColor(const Napi::CallbackInfo&, const Napi::Value& val) {
    m_clearColor = imgui::asColor(val);
}
