#include <iostream>
#include <napi.h>

Napi::Value helloWorld(const Napi::CallbackInfo& info)
{
    std::cout << "hello, world!" << std::endl;
    return info.Env().Undefined();
}

Napi::Object initModule(Napi::Env env, Napi::Object exports)
{
    exports.Set("helloWorld", Napi::Function::New(env, helloWorld));
    return exports;
}

NODE_API_MODULE(ml64fxModule, initModule)
