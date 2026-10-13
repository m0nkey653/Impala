#include "sprite_atlas.h"
#include "SDL3/SDL_surface.h"

SpriteAtlas::SpriteAtlas(const std::string& name, const std::string& sheetPath,
                         const std::string& atlasPath)
    : _name(name), _sheetPath(sheetPath), _atlasPath(atlasPath) {}

std::string SpriteAtlas::Name() {
    return _name;
}

void SpriteAtlas::Load() {
    SDL_Surface* sheet = SDL_LoadPNG(_sheetPath.c_str());
}