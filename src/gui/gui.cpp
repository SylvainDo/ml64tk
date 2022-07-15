#include "gui/dialogs.hpp"
#include "gui/gui.hpp"

namespace gui {

Napi::Object initialize(Napi::Env env, Napi::Object exports) {
    using namespace dialogs;
    exports.Set("getOpenFileName", Napi::Function::New(env, getOpenFileName));
    exports.Set("getOpenFileNames", Napi::Function::New(env, getOpenFileNames));
    exports.Set("getSaveFileName", Napi::Function::New(env, getSaveFileName));
    exports.Set("getExistingDirectory", Napi::Function::New(env, getExistingDirectory));
    exports.Set("getExistingDirectories", Napi::Function::New(env, getExistingDirectories));
    exports.Set("showMessageBox", Napi::Function::New(env, showMessageBox));

    return exports;
}

}
