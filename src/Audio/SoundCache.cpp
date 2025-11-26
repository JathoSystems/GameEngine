#include "Audio/SoundCache.h"

void SoundCache::addSound(const std::string& key, const DecodedAudio& audio) {
    cachedSounds[key] = audio;
}

DecodedAudio SoundCache::getSound(const std::string& key) const {
    auto it = cachedSounds.find(key);
    if (it != cachedSounds.end()) {
        return it->second;
    }
    return DecodedAudio{};
}

bool SoundCache::hasSound(const std::string& key) const {
    return cachedSounds.find(key) != cachedSounds.end();
}

void SoundCache::removeSound(const std::string& key) {
    cachedSounds.erase(key);
}
