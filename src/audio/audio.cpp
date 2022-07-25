#include "audio/audio.hpp"
#include "audio/listener.hpp"
#include "audio/music.hpp"
#include "audio/sound.hpp"
#include "audio/soundBuffer.hpp"

namespace audio {

Napi::Object initialize(Napi::Env env, Napi::Object exports) {
    Listener::initialize(env, exports);
    Music::initialize(env, exports);
    Sound::initialize(env, exports);
    SoundBuffer::initialize(env, exports);

    return exports;
}

}
