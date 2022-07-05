#include "core/largeInteger.hpp"

Napi::Object initModule(Napi::Env env, Napi::Object exports) {
    core::SignedLargeInteger::initialize(env, exports);
    core::UnsignedLargeInteger::initialize(env, exports);

    return exports;
}

NODE_API_MODULE(ml64fxModule, initModule)
