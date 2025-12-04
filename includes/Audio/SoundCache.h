#ifndef GAMEENGINE_SOUNDCACHE_H
#define GAMEENGINE_SOUNDCACHE_H

#include <map>
#include <string>
#include "DecodedAudio.h"

class SoundCache {
private:
    std::map<std::string, DecodedAudio> cachedSounds;

public:
    void addSound(const std::string& key, const DecodedAudio& audio);
    DecodedAudio getSound(const std::string& key) const;
    bool hasSound(const std::string& key) const;
    void removeSound(const std::string& key);
};

#endif
