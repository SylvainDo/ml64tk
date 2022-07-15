#include "core/type/convert.hpp"
#include "gui/gui.hpp"
#include "imgui/viewport.hpp"

#include <optional>
#include <vector>

#include <imgui.h>

#include <shlobj_core.h>
#pragma comment(lib, "comctl32")

using namespace core::type::convert;

namespace gui::dialogs {

#define u16str2wstr(x) reinterpret_cast<LPCWSTR>(x.c_str())

std::u16string makeFilterString(const Napi::Array& val) {
    std::u16string str;
    for (std::size_t i{}, length = val.Length(); i < length; ++i)
        str += std::u16string{ u"*." } + asStrUtf16(val.Get(i)) + u";";
    str.pop_back();
    return str;
}

std::vector<COMDLG_FILTERSPEC> makeFilterSpecs(std::vector<std::u16string>& stringsCache, const Napi::Array& val) {
    std::vector<COMDLG_FILTERSPEC> filterSpecs;
    for (std::size_t i{}, length = val.Length(); i < length;) {
        stringsCache.push_back(asStrUtf16(val.Get(i++)));
        stringsCache.push_back(makeFilterString(valueAsArray(val.Get(i++))));
        filterSpecs.push_back({ u16str2wstr(stringsCache[stringsCache.size() - 2]),
            u16str2wstr(stringsCache[stringsCache.size() - 1]) });
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
        m_parent = static_cast<HWND>(imgui::Viewport::get(val)->PlatformHandleRaw);
    }

    void setTitle(const Napi::Value& val) {
        m_diag->SetTitle(u16str2wstr(asStrUtf16(val)));
    }

    void setFolder(const Napi::Value& val) {
        std::unique_ptr<ITEMIDLIST, decltype(&ilFree)> idl{ nullptr, ilFree };
        auto pidl = idl.get();
        SHParseDisplayName(u16str2wstr(asStrUtf16(val)), nullptr, &pidl, SFGAO_FOLDER, nullptr);
        std::unique_ptr<IShellItem, decltype(&shellItemRelease)> si{ nullptr, shellItemRelease };
        auto psi = si.get();
        SHCreateShellItem(nullptr, nullptr, pidl, &psi);
        m_diag->SetFolder(psi);
    }

    void setFileTypes(const Napi::Value& val) {
        std::vector<std::u16string> stringsCache;
        const auto filterSpecs = makeFilterSpecs(stringsCache, valueAsArray(val));
        m_diag->SetFileTypes(filterSpecs.size(), filterSpecs.data());
    }

    void setOkButtonLabel(const Napi::Value& val) {
        m_diag->SetOkButtonLabel(u16str2wstr(asStrUtf16(val)));
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
        return FAILED(m_diag->GetFileTypeIndex(&fileTypeIndex)) || fileTypeIndex == 0 ? m_env.Undefined() : fromU32(m_env, fileTypeIndex - 1);
    }

    Napi::Value getShellItemFileName(IShellItem* psi) {
        std::unique_ptr<wchar_t, decltype(&filePathFree)> filePath{ nullptr, filePathFree };
        auto pFilePath = filePath.get();
        psi->GetDisplayName(SIGDN_FILESYSPATH, &pFilePath);
        return fromStrUtf16(m_env, reinterpret_cast<const char16_t*>(pFilePath));
    }

    Napi::Value getResultFileName() {
        std::unique_ptr<IShellItem, decltype(&shellItemRelease)> si{ nullptr, shellItemRelease };
        auto psi = si.get();
        return SUCCEEDED(m_diag->GetResult(&psi)) ? getShellItemFileName(psi) : m_env.Undefined();
    }

    Napi::Value getResultFileNames() {
        std::unique_ptr<IShellItemArray, decltype(&shellItemArrayRelease)> sia{ nullptr, shellItemArrayRelease };
        auto psia = sia.get();
        if (SUCCEEDED(m_diag->GetResults(&psia))) {
            auto val = Napi::Array::New(m_env);
            DWORD numItems;
            psia->GetCount(&numItems);
            for (DWORD i{}; i < numItems; ++i) {
                std::unique_ptr<IShellItem, decltype(&shellItemRelease)> si{ nullptr, shellItemRelease };
                auto psi = si.get();
                psia->GetItemAt(i, &psi);
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
        if (const auto v = obj.Get("okButtonLabel"); !v.IsUndefined()) diag.setOkButtonLabel(v);
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

TASKDIALOG_COMMON_BUTTON_FLAGS convertButtons(int buttons) {
    switch (buttons) {
    default: case 0: return TDCBF_OK_BUTTON;
    case 1: return TDCBF_CANCEL_BUTTON;
    case 2: return TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON;
    case 3: return TDCBF_YES_BUTTON | TDCBF_NO_BUTTON;
    case 4: return TDCBF_CLOSE_BUTTON;
    }
}

LPCWSTR convertIcon(int icon) {
    switch (icon) {
    default: case 0: return 0;
    case 1: return TD_WARNING_ICON;
    case 2: return TD_ERROR_ICON;
    case 3: return TD_INFORMATION_ICON;
    }
}

int convertResult(int result) {
    switch (result) {
    default: case 0: case IDOK: return 0;
    case IDCANCEL: return 1;
    case IDYES: return 2;
    case IDNO: return 3;
    case IDCLOSE: return 4;
    }
}

Napi::Value showMessageBox(const Napi::CallbackInfo& info) {
    std::optional<HWND> parent;
    std::optional<std::u16string> title, mainInstruction, content;
    std::optional<TASKDIALOG_COMMON_BUTTON_FLAGS> buttons;
    std::optional<PCWSTR> icon;

    if (!info[0].IsUndefined()) {
        const auto obj = valueAsObject(info[0]);
        if (const auto v = obj.Get("parent"); !v.IsUndefined()) parent = static_cast<HWND>(imgui::Viewport::get(v)->PlatformHandleRaw);
        if (const auto v = obj.Get("title"); !v.IsUndefined()) title = asStrUtf16(v);
        if (const auto v = obj.Get("mainInstruction"); !v.IsUndefined()) mainInstruction = asStrUtf16(v);
        if (const auto v = obj.Get("content"); !v.IsUndefined()) content = asStrUtf16(v);
        if (const auto v = obj.Get("buttons"); !v.IsUndefined()) buttons = convertButtons(asS32(v));
        if (const auto v = obj.Get("icon"); !v.IsUndefined()) icon = convertIcon(asS32(v));
    }

    int result;
    TaskDialog(
        parent.has_value() ? parent.value() : nullptr,
        GetModuleHandle(nullptr),
        title.has_value() ? u16str2wstr(title.value()) : nullptr,
        mainInstruction.has_value() ? u16str2wstr(mainInstruction.value()) : nullptr,
        content.has_value() ? u16str2wstr(content.value()) : nullptr,
        buttons.has_value() ? buttons.value() : 0,
        icon.has_value() ? icon.value() : 0,
        &result);
    return fromS32(info.Env(), convertResult(result));
}

}
