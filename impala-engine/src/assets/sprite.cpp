#include "sprite.h"

std::string Sprite::Name() {
    return _name;
}

void Sprite::Load() {
    auto sheet = SDL_LoadSurface(_spritePath.c_str());
    if (!sheet) {
        SDL_Log("Failed to load surface '%s': %s", _spritePath.c_str(), SDL_GetError());
    }
}