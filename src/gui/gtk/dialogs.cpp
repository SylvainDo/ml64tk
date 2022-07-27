#include "core/convert.hpp"
#include "gui/gtk/util.hpp"
#include "gui/dialogs.hpp"
#include "imgui/viewport.hpp"

#include <optional>
#include <thread>

#include <fmt/format.h>
#include <gdk/gdk.h>
#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif
#include <imgui.h>
#include <SDL_video.h>

using namespace core;
using namespace imgui;

namespace gui::dialogs {

#ifdef GDK_WINDOWING_X11
static void positionDialog(GtkWidget* widget, gpointer userData) {
    const auto parentWindow = static_cast<SDL_Window*>(userData);
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> widgetWindow{ nullptr, SDL_DestroyWindow };

    const auto x11Window = gdk_x11_surface_get_xid(gtk_native_get_surface(GTK_NATIVE(widget)));
    const auto x11Display = gdk_x11_display_get_xdisplay(gtk_widget_get_display(widget));
    XWindowAttributes attr{};
    XGetWindowAttributes(x11Display, x11Window, &attr);
    widgetWindow.reset(SDL_CreateWindowFrom(reinterpret_cast<void*>(x11Window)));

    if (parentWindow) {
        int x, y, w, h;
        SDL_GetWindowPosition(parentWindow, &x, &y);
        SDL_GetWindowSize(parentWindow, &w, &h);
        x += w / 2 - attr.width / 2;
        y += h / 2 - attr.height / 2;
        SDL_SetWindowPosition(widgetWindow.get(), x, y);
    }

    SDL_RaiseWindow(widgetWindow.get());
    SDL_SetWindowAlwaysOnTop(widgetWindow.get(), SDL_TRUE);
}
#endif

static void retrieveDialogResponse(GtkDialog*, gint responseId, gpointer userData) {
    auto pResponseId = static_cast<gint*>(userData);
    *pResponseId = responseId;
}

static const char* fileChooserActionToCStr(GtkFileChooserAction action) {
    switch (action) {
    case GTK_FILE_CHOOSER_ACTION_OPEN: return "Open";
    case GTK_FILE_CHOOSER_ACTION_SAVE: return "Save as";
    case GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER: return "Select folder";
    default: return "";
    }
}

GtkResponseType runDialog(SDL_Window* parent, GtkWindowAutoPtr_t dialog, bool isMessageDialog) {
    GtkWindowAutoPtr(w, gtk_window_new());
    gtk_window_set_title(GTK_WINDOW(w.get()), isMessageDialog ? "Message" :
        fileChooserActionToCStr(gtk_file_chooser_get_action(GTK_FILE_CHOOSER(dialog.get()))));
    gtk_window_set_decorated(GTK_WINDOW(w.get()), false);
    gtk_window_set_default_size(GTK_WINDOW(w.get()), 1, 1);

    g_signal_connect(w.get(), "show", G_CALLBACK([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
    }), nullptr);
#ifdef GDK_WINDOWING_X11
    g_signal_connect(w.get(), "show", G_CALLBACK(positionDialog), parent);
    if (!isMessageDialog) g_signal_connect(dialog.get(), "show", G_CALLBACK(positionDialog), parent);
#endif

    gtk_widget_realize(w.get());
    gtk_widget_show(w.get());
    gtk_window_set_transient_for(GTK_WINDOW(dialog.get()), GTK_WINDOW(w.get()));
    gtk_widget_show(dialog.get());

    gint responseId{};
    g_signal_connect(dialog.get(), "response", G_CALLBACK(retrieveDialogResponse), &responseId);

    while (responseId == 0) {
        g_main_context_iteration(nullptr, true);
        std::this_thread::yield();
    }

    if (parent) {
        SDL_RaiseWindow(parent);
        SDL_SetWindowInputFocus(parent);
    }

    return static_cast<GtkResponseType>(responseId);
}

static std::string makeFilterString(const Napi::Array& val) {
    std::string str;
    for (std::size_t i{}, length = val.Length(); i < length; ++i)
        str += fmt::format("*.{};", asStrUtf8(val.Get(i)));
    str.pop_back();
    return str;
}

struct FileDialogResult {
    GObjectAutoPtr(GFile, file, nullptr);
    GObjectAutoPtr(GListModel, files, nullptr);
    GtkFileFilter* filter;
};

static void retrieveFileDialogResult(GtkWidget* dialog, gint responseId, gpointer userData) {
    auto fileResult = static_cast<FileDialogResult*>(userData);
    if (responseId == GTK_RESPONSE_ACCEPT) {
        if (gtk_file_chooser_get_select_multiple(GTK_FILE_CHOOSER(dialog)))
            fileResult->files.reset(gtk_file_chooser_get_files(GTK_FILE_CHOOSER(dialog)));
        else
            fileResult->file.reset(gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog)));
        fileResult->filter = gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(dialog));
    }
}

Napi::Value runFileDialog(const Napi::CallbackInfo& info, GtkFileChooserAction action, bool selectMultiple) {
    SDL_Window* parent{};
    std::optional<std::string> title, currentFolder;
    Napi::Array filters;
    GtkWindowAutoPtr(dialog, nullptr);
    FileDialogResult fileResult{};
    std::vector<GtkFileFilter*> filtersRef;

    if (!info[0].IsUndefined()) {
        const auto obj = valueAsObject(info[0]);
        if (const auto v = obj.Get("parent"); !v.IsUndefined()) parent = static_cast<SDL_Window*>(Viewport::get(v)->PlatformHandle);
        if (const auto v = obj.Get("title"); !v.IsUndefined()) title = asStrUtf8(v);
        if (const auto v = obj.Get("currentFolder"); !v.IsUndefined()) currentFolder = asStrUtf8(v);
        if (const auto v = obj.Get("filters"); !v.IsUndefined()) filters = valueAsArray(v);
    }

    dialog.reset(gtk_file_chooser_dialog_new(
        title.has_value() ? title.value().c_str() : nullptr,
        nullptr,
        action,
        "_Cancel",
        GTK_RESPONSE_CANCEL,
        action != GTK_FILE_CHOOSER_ACTION_SAVE ? "_Open" : "_Save",
        GTK_RESPONSE_ACCEPT,
        nullptr));
    g_signal_connect(dialog.get(), "response", G_CALLBACK(retrieveFileDialogResult), &fileResult);

    if (currentFolder.has_value()) {
        const GObjectAutoPtr(GFile, file, g_file_new_for_path(currentFolder.value().c_str()));
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog.get()), file.get(), nullptr);
    }

    if (!filters.IsUndefined()) {
        for (std::size_t i{}, length = filters.Length(); i < length; i += 2) {
            const GObjectAutoPtr(GtkFileFilter, filter, gtk_file_filter_new());
            const auto extensions = valueAsArray(filters.Get(i + 1));
            gtk_file_filter_set_name(filter.get(), fmt::format("{} ({})", asStrUtf8(filters.Get(i)), makeFilterString(extensions)).c_str());
            for (std::size_t j{}, length2 = extensions.Length(); j < length2; ++j)
                gtk_file_filter_add_suffix(filter.get(), asStrUtf8(extensions.Get(j)).c_str());
            gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog.get()), filter.get());
            filtersRef.push_back(filter.get());
        }
    }

    if (selectMultiple)
        gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog.get()), selectMultiple);

    const auto response = runDialog(parent, std::move(dialog), false);
    const auto selectedFilter = !fileResult.filter || filtersRef.empty() ? info.Env().Undefined() :
        fromS32(info.Env(), static_cast<int>(std::distance(filtersRef.begin(), std::find(filtersRef.begin(), filtersRef.end(), fileResult.filter))));

    if (response == GTK_RESPONSE_ACCEPT) {
        if (!selectMultiple) {
            const auto filename = fromStrUtf8(info.Env(), stringFromGString(g_file_get_path(fileResult.file.get())));
            if (action == GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER) return filename;
            else {
                auto obj = Napi::Object::New(info.Env());
                obj.Set("selectedFilter", selectedFilter);
                obj.Set("filename", filename);
                return obj;
            }
        }
        else {
            const auto files = static_cast<GListModel*>(fileResult.files.get());
            auto filenames = Napi::Array::New(info.Env());
            for (std::size_t i{}; i < g_list_model_get_n_items(files); ++i) {
                const GObjectAutoPtr(GFile, file, static_cast<GFile*>(g_list_model_get_item(files, i)));
                filenames.Set(i, fromStrUtf8(info.Env(), stringFromGString(g_file_get_path(file.get()))));
            }
            if (action == GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER) return filenames;
            else {
                auto obj = Napi::Object::New(info.Env());
                obj.Set("selectedFilter", selectedFilter);
                obj.Set("filenames", filenames);
                return obj;
            }
        }
    }
    else {
        if (action == GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER) return info.Env().Undefined();
        else {
            auto obj = Napi::Object::New(info.Env());
            obj.Set("selectedFilter", selectedFilter);
            obj.Set(selectMultiple ? "filenames" : "filename", info.Env().Undefined());
            return obj;
        }
    }
}

Napi::Value getOpenFileName(const Napi::CallbackInfo& info) {
    return runFileDialog(info, GTK_FILE_CHOOSER_ACTION_OPEN, false);
}

Napi::Value getOpenFileNames(const Napi::CallbackInfo& info) {
    return runFileDialog(info, GTK_FILE_CHOOSER_ACTION_OPEN, true);
}

Napi::Value getSaveFileName(const Napi::CallbackInfo& info) {
    return runFileDialog(info, GTK_FILE_CHOOSER_ACTION_SAVE, false);
}

Napi::Value getExistingDirectory(const Napi::CallbackInfo& info) {
    return runFileDialog(info, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, false);
}

Napi::Value getExistingDirectories(const Napi::CallbackInfo& info) {
    return runFileDialog(info, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, true);
}

static GtkMessageType convertType(MessageBoxType type) {
    switch (type) {
    default: case MessageBoxType::Other: return GTK_MESSAGE_OTHER;
    case MessageBoxType::Warning: return GTK_MESSAGE_WARNING;
    case MessageBoxType::Error: return GTK_MESSAGE_ERROR;
    case MessageBoxType::Information: return GTK_MESSAGE_INFO;
    }
}

static GtkButtonsType convertButtons(MessageBoxButtons buttons) {
    switch (buttons) {
    default: case MessageBoxButtons::OK: return GTK_BUTTONS_OK;
    case MessageBoxButtons::Cancel: return GTK_BUTTONS_CANCEL;
    case MessageBoxButtons::OKCancel: return GTK_BUTTONS_OK_CANCEL;
    case MessageBoxButtons::YesNo: return GTK_BUTTONS_YES_NO;
    case MessageBoxButtons::Close: return GTK_BUTTONS_CLOSE;
    }
}

static MessageBoxResponse convertResponse(GtkResponseType response) {
    switch (response) {
    default: case GTK_RESPONSE_OK: return MessageBoxResponse::OK;
    case GTK_RESPONSE_CANCEL: return MessageBoxResponse::Cancel;
    case GTK_RESPONSE_YES: return MessageBoxResponse::Yes;
    case GTK_RESPONSE_NO: return MessageBoxResponse::No;
    case GTK_RESPONSE_CLOSE: return MessageBoxResponse::Close;
    }
}

Napi::Value showMessageBox(const Napi::CallbackInfo& info) {
    SDL_Window* parent{};
    std::optional<std::string> title, subtitle, content;
    std::optional<GtkMessageType> type;
    std::optional<GtkButtonsType> buttons;
    GtkWindowAutoPtr(dialog, nullptr);

    if (!info[0].IsUndefined()) {
        const auto obj = valueAsObject(info[0]);
        if (const auto v = obj.Get("parent"); !v.IsUndefined()) parent = static_cast<SDL_Window*>(Viewport::get(v)->PlatformHandle);
        if (const auto v = obj.Get("title"); !v.IsUndefined()) title = asStrUtf8(v);
        if (const auto v = obj.Get("subtitle"); !v.IsUndefined()) subtitle = asStrUtf8(v);
        if (const auto v = obj.Get("content"); !v.IsUndefined()) content = asStrUtf8(v);
        if (const auto v = obj.Get("type"); !v.IsUndefined()) type = convertType(static_cast<MessageBoxType>(asS32(v)));
        if (const auto v = obj.Get("buttons"); !v.IsUndefined()) buttons = convertButtons(static_cast<MessageBoxButtons>(asS32(v)));
    }

    if (subtitle.has_value()) {
        dialog.reset(gtk_message_dialog_new(
            nullptr,
            GTK_DIALOG_MODAL,
            type.has_value() ? type.value() : GTK_MESSAGE_OTHER,
            buttons.has_value() ? buttons.value() : GTK_BUTTONS_OK,
            "%s",
            title.has_value() ? title.value().c_str() : (parent ? SDL_GetWindowTitle(parent) : "")));
        gtk_message_dialog_format_secondary_markup(
            GTK_MESSAGE_DIALOG(dialog.get()),
            "<span size=\"large\" foreground=\"#4c88ff\">%s</span>\r\r%s",
            subtitle.value().c_str(),
            content.has_value() ? content.value().c_str() : "");
        gtk_window_set_default_size(GTK_WINDOW(dialog.get()), 442, 189);
    }
    else {
        dialog.reset(gtk_message_dialog_new(
            nullptr,
            GTK_DIALOG_MODAL,
            type.has_value() ? type.value() : GTK_MESSAGE_OTHER,
            buttons.has_value() ? buttons.value() : GTK_BUTTONS_OK,
            "%s",
            title.has_value() ? title.value().c_str() : (parent ? SDL_GetWindowTitle(parent) : "")));
        gtk_message_dialog_format_secondary_text(
            GTK_MESSAGE_DIALOG(dialog.get()),
            "%s",
            content.has_value() ? content.value().c_str() : "");
        gtk_window_set_default_size(GTK_WINDOW(dialog.get()), 442, 156);
    }
    return fromS32(info.Env(), static_cast<int>(convertResponse(runDialog(parent, std::move(dialog), true))));
}

}
