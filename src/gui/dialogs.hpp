#pragma once

#include <napi.h>

namespace gui::dialogs {

Napi::Value getOpenFileName(const Napi::CallbackInfo& info);
Napi::Value getOpenFileNames(const Napi::CallbackInfo& info);
Napi::Value getSaveFileName(const Napi::CallbackInfo& info);
Napi::Value getExistingDirectory(const Napi::CallbackInfo& info);
Napi::Value getExistingDirectories(const Napi::CallbackInfo& info);
Napi::Value showMessageBox(const Napi::CallbackInfo& info);

}
