#include "gfx/gfx.hpp"
#include "gfx/texture.hpp"

namespace gfx {

Napi::Object initialize(Napi::Env env, Napi::Object exports) {
    Texture::initialize(env, exports);

    return exports;
}

}
