#include "asset_database.h"
#include "sprite_atlas.h"

void AssetDatabase::AddSpriteAtlas(const std::string& name, const std::string& atlasPath) {
    auto it = _spriteAtlases.find(name);
    if (it != _spriteAtlases.end()) {
        // TODO: Log that the sprite atlas already exists
        return;
    }

    auto atlas = std::make_unique<SpriteAtlas>(name, atlasPath);
    atlas->Initialize();
    _spriteAtlases[name] = std::move(atlas);
}

impala::ISpriteAtlas* AssetDatabase::GetSpriteAtlas(const std::string& name) {
    // Implementation for retrieving a sprite atlas
    auto it = _spriteAtlases.find(name);
    if (it != _spriteAtlases.end()) {
        return it->second.get();
    }

    // TODO: Log that the sprite atlas was not found
    return nullptr;
}