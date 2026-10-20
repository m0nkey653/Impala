#include "asset_database.h"
#include "sprite_atlas.h"

void AssetDatabase::AddSpriteAtlas(const std::string& name, const std::string& atlasPath) {
    auto it = _spriteAtlases.find(name);
    if (it != _spriteAtlases.end()) {
        impala::log::Warn(impala::log::LogCategory::Assets, "Sprite atlas '{}' already exists",
                          name);
        return;
    }

    auto atlas = std::make_unique<SpriteAtlas>(name, atlasPath);
    atlas->Initialize();
    _spriteAtlases[name] = std::move(atlas);
}

impala::ISpriteAtlas* AssetDatabase::GetSpriteAtlas(const std::string& name) {
    auto it = _spriteAtlases.find(name);
    if (it != _spriteAtlases.end()) {
        return it->second.get();
    }

    // leave this a null pointer for now
    // we'll decide later whether to throw or provide a default atlas here
    impala::log::Error(impala::log::LogCategory::Assets, "Sprite atlas '{}' not found", name);
    return nullptr;
}