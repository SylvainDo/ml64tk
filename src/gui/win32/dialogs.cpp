#include "core/type/convert.hpp"
#include "gui/dialogs.hpp"
#include "gui/gui.hpp"
#include "imgui/viewport.hpp"

#include <optional>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <imgui.h>
#include <SDL_video.h>

#include <shlobj_core.h>
#pragma comment(lib, "comctl32")

using namespace core::type::convert;
using namespace imgui;

namespace gui::dialogs {

static std::wstring s2ws(const std::string& str) {
    if (str.empty()) return {};
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
    if (size == 0) return {};
    std::wstring wstr(size, '\0');
    size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), wstr.data(), static_cast<int>(wstr.size()));
    if (size == 0) return {};
    return wstr;
}

static std::string makeFilterString(const Napi::Array& val) {
    std::string str;
    for (std::size_t i{}, length = val.Length(); i < length; ++i)
        str += fmt::format("*.{};", asStrUtf8(val.Get(i)));
    str.pop_back();
    return str;
}

static std::vector<COMDLG_FILTERSPEC> makeFilterSpecs(std::vector<std::wstring>& stringsCache, const Napi::Array& val) {
    std::vector<COMDLG_FILTERSPEC> filterSpecs(val.Length() / 2);
    stringsCache.resize(val.Length());
    for (std::size_t i{}, j{}, length = val.Length(); i < length; ++j) {
        stringsCache[i] = s2ws(asStrUtf8(val.Get(i))); ++i;
        stringsCache[i] = s2ws(makeFilterString(valueAsArray(val.Get(i)))); ++i;
        filterSpecs[j] = { stringsCache[i - 2].c_str(), stringsCache[i - 1].c_str() };
    }
    return filterSpecs;
}

template <class TDialog>
class FileDialog {
private:
    static void ilFree(LPITEMIDLIST pidl) {
        ILFree(pidl);
    }

    static void shellItemRelease(IShellItem* psi) {
        psi->Release();
    }

    static void shellItemArrayRelease(IShellItemArray* psia) {
        psia->Release();
    }

    static void filePathFree(wchar_t* filePath) {
        CoTaskMemFree(filePath);
    }

public:
    FileDialog(Napi::Env env, const CLSID& clsid, const IID& iid) : m_env{ env } {
        if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))
            || FAILED(CoCreateInstance(clsid, nullptr, CLSCTX_ALL, iid, reinterpret_cast<void**>(&m_diag))))
            throw Napi::Error::New(env, "failed to initialize file dialog");
    }

    ~FileDialog() {
        m_diag->Release();
        CoUninitialize();
    }

    void setOptions(FILEOPENDIALOGOPTIONS val) {
        FILEOPENDIALOGOPTIONS options;
        m_diag->GetOptions(&options);
        m_diag->SetOptions(options | val);
    }

    void setParent(const Napi::Value& val) {
        m_parent = static_cast<HWND>(Viewport::get(val)->PlatformHandleRaw);
    }

    void setTitle(const Napi::Value& val) {
        m_diag->SetTitle(s2ws(asStrUtf8(val)).c_str());
    }

    void setFolder(const Napi::Value& val) {
        std::unique_ptr<ITEMIDLIST, decltype(&ilFree)> idl{ nullptr, ilFree };
        ITEMIDLIST* pidl{};
        SHParseDisplayName(s2ws(asStrUtf8(val)).c_str(), nullptr, &pidl, SFGAO_FOLDER, nullptr);
        idl.reset(pidl);
        std::unique_ptr<IShellItem, decltype(&shellItemRelease)> si{ nullptr, shellItemRelease };
        IShellItem* psi{};
        SHCreateShellItem(nullptr, nullptr, pidl, &psi);
        si.reset(psi);
        m_diag->SetFolder(psi);
    }

    void setFileTypes(const Napi::Value& val) {
        std::vector<std::wstring> stringsCache;
        const auto filterSpecs = makeFilterSpecs(stringsCache, valueAsArray(val));
        m_diag->SetFileTypes(filterSpecs.size(), filterSpecs.data());
    }

    void show() {
        m_diag->Show(m_parent);
    }

    Napi::Value getResultFile() {
        auto obj = Napi::Object::New(m_env);
        obj.Set("selectedFilter", getFileTypeIndex());
        obj.Set("filename", getResultFileName());
        return obj;
    }

    Napi::Value getResultFiles() {
        auto obj = Napi::Object::New(m_env);
        obj.Set("selectedFilter", getFileTypeIndex());
        obj.Set("filenames", getResultFileNames());
        return obj;
    }

    Napi::Value getResultFolder() {
        return getResultFileName();
    }

    Napi::Value getResultFolders() {
        return getResultFileNames();
    }

private:
    Napi::Env m_env;
    TDialog* m_diag;
    HWND m_parent{};

    Napi::Value getFileTypeIndex() {
        UINT fileTypeIndex;
        return FAILED(m_diag->GetFileTypeIndex(&fileTypeIndex)) || fileTypeIndex == 0 ?
            m_env.Undefined() : fromU32(m_env, fileTypeIndex - 1);
    }

    Napi::Value getShellItemFileName(IShellItem* psi) {
        std::unique_ptr<wchar_t, decltype(&filePathFree)> filePath{ nullptr, filePathFree };
        wchar_t* pFilePath{};
        psi->GetDisplayName(SIGDN_FILESYSPATH, &pFilePath);
        if (!pFilePath) psi->GetDisplayName(SIGDN_PARENTRELATIVEFORADDRESSBAR, &pFilePath);
        filePath.reset(pFilePath);
        return fromStrUtf16(m_env, reinterpret_cast<const char16_t*>(pFilePath));
    }

    Napi::Value getResultFileName() {
        std::unique_ptr<IShellItem, decltype(&shellItemRelease)> si{ nullptr, shellItemRelease };
        IShellItem* psi{};
        if (SUCCEEDED(m_diag->GetResult(&psi))) {
            si.reset(psi);
            return getShellItemFileName(psi);
        }
        return m_env.Undefined();
    }

    Napi::Value getResultFileNames() {
        std::unique_ptr<IShellItemArray, decltype(&shellItemArrayRelease)> sia{ nullptr, shellItemArrayRelease };
        IShellItemArray* psia{};
        if (SUCCEEDED(m_diag->GetResults(&psia))) {
            sia.reset(psia);
            auto val = Napi::Array::New(m_env);
            DWORD numItems;
            psia->GetCount(&numItems);
            for (DWORD i{}; i < numItems; ++i) {
                std::unique_ptr<IShellItem, decltype(&shellItemRelease)> si{ nullptr, shellItemRelease };
                IShellItem* psi{};
                psia->GetItemAt(i, &psi);
                si.reset(psi);
                val.Set(i, getShellItemFileName(psi));
            }
            return val;
        }
        return m_env.Undefined();
    }
};

template <class TDialog, bool PickFolder, bool MultiSelect>
Napi::Value execFileDialog(const Napi::CallbackInfo& info, const CLSID& clsid, const IID& iid) {
    FileDialog<TDialog> diag{ info.Env(), clsid, iid };
    FILEOPENDIALOGOPTIONS options{};
    if constexpr (PickFolder) options |= FOS_PICKFOLDERS;
    if constexpr (MultiSelect) options |= FOS_ALLOWMULTISELECT;
    diag.setOptions(options);

    if (!info[0].IsUndefined()) {
        const auto obj = valueAsObject(info[0]);
        if (const auto v = obj.Get("parent"); !v.IsUndefined()) diag.setParent(v);
        if (const auto v = obj.Get("title"); !v.IsUndefined()) diag.setTitle(v);
        if (const auto v = obj.Get("currentFolder"); !v.IsUndefined()) diag.setFolder(v);
        if (const auto v = obj.Get("filters"); !v.IsUndefined()) diag.setFileTypes(v);
    }
    diag.show();

    if constexpr (PickFolder && MultiSelect) return diag.getResultFolders();
    else if constexpr (PickFolder) return diag.getResultFolder();
    else if constexpr (MultiSelect) return diag.getResultFiles();
    else return diag.getResultFile();
}

Napi::Value getOpenFileName(const Napi::CallbackInfo& info) {
    return execFileDialog<IFileOpenDialog, false, false>(info, CLSID_FileOpenDialog, IID_IFileOpenDialog);
}

Napi::Value getOpenFileNames(const Napi::CallbackInfo& info) {
    return execFileDialog<IFileOpenDialog, false, true>(info, CLSID_FileOpenDialog, IID_IFileOpenDialog);
}

Napi::Value getSaveFileName(const Napi::CallbackInfo& info) {
    return execFileDialog<IFileSaveDialog, false, false>(info, CLSID_FileSaveDialog, IID_IFileSaveDialog);
}

Napi::Value getExistingDirectory(const Napi::CallbackInfo& info) {
    return execFileDialog<IFileOpenDialog, true, false>(info, CLSID_FileOpenDialog, IID_IFileOpenDialog);
}

Napi::Value getExistingDirectories(const Napi::CallbackInfo& info) {
    return execFileDialog<IFileOpenDialog, true, true>(info, CLSID_FileOpenDialog, IID_IFileOpenDialog);
}

static LPCWSTR convertType(MessageBoxType type) {
    switch (type) {
    default: case MessageBoxType::Other: return nullptr;
    case MessageBoxType::Warning: return TD_WARNING_ICON;
    case MessageBoxType::Error: return TD_ERROR_ICON;
    case MessageBoxType::Information: return TD_INFORMATION_ICON;
    }
}

static TASKDIALOG_COMMON_BUTTON_FLAGS convertButtons(MessageBoxButtons buttons) {
    switch (buttons) {
    default: case MessageBoxButtons::OK: return TDCBF_OK_BUTTON;
    case MessageBoxButtons::Cancel: return TDCBF_CANCEL_BUTTON;
    case MessageBoxButtons::OKCancel: return TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON;
    case MessageBoxButtons::YesNo: return TDCBF_YES_BUTTON | TDCBF_NO_BUTTON;
    case MessageBoxButtons::Close: return TDCBF_CLOSE_BUTTON;
    }
}

static MessageBoxResponse convertResponse(int response) {
    switch (response) {
    default: case 0: case IDOK: return MessageBoxResponse::OK;
    case IDCANCEL: return MessageBoxResponse::Cancel;
    case IDYES: return MessageBoxResponse::Yes;
    case IDNO: return MessageBoxResponse::No;
    case IDCLOSE: return MessageBoxResponse::Close;
    }
}

Napi::Value showMessageBox(const Napi::CallbackInfo& info) {
    ImGuiViewport* parent{};
    std::optional<std::wstring> title, subtitle, content;
    std::optional<PCWSTR> type;
    std::optional<TASKDIALOG_COMMON_BUTTON_FLAGS> buttons;

    if (!info[0].IsUndefined()) {
        const auto obj = valueAsObject(info[0]);
        if (const auto v = obj.Get("parent"); !v.IsUndefined()) parent = Viewport::get(v);
        if (const auto v = obj.Get("title"); !v.IsUndefined()) title = s2ws(asStrUtf8(v));
        if (const auto v = obj.Get("subtitle"); !v.IsUndefined()) subtitle = s2ws(asStrUtf8(v));
        if (const auto v = obj.Get("content"); !v.IsUndefined()) content = s2ws(asStrUtf8(v));
        if (const auto v = obj.Get("type"); !v.IsUndefined()) type = convertType(static_cast<MessageBoxType>(asS32(v)));
        if (const auto v = obj.Get("buttons"); !v.IsUndefined()) buttons = convertButtons(static_cast<MessageBoxButtons>(asS32(v)));
    }

    int response;
    TaskDialog(
        parent ? static_cast<HWND>(parent->PlatformHandleRaw) : nullptr,
        GetModuleHandle(nullptr),
        title.has_value() ? title.value().c_str() :
            (parent ? s2ws(SDL_GetWindowTitle(static_cast<SDL_Window*>(parent->PlatformHandle))).c_str() : nullptr),
        subtitle.has_value() ? subtitle.value().c_str() : nullptr,
        content.has_value() ? content.value().c_str() : nullptr,
        buttons.has_value() ? buttons.value() : 0,
        type.has_value() ? type.value() : 0,
        &response);
    return fromS32(info.Env(), static_cast<int>(convertResponse(response)));
}

}
