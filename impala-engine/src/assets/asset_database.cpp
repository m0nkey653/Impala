#include "asset_database.h"
#include "impala/log.h"
#include "sprite_atlas.h"

using namespace impala;

void AssetDatabase::AddSpriteAtlas(const std::string& atlasPath) {
    auto it = _spriteAtlases.find(atlasPath);

    if (it != _spriteAtlases.end()) {
        printf("Sprite atlas '%s' already exists\n", atlasPath.c_str());
    }

    auto fullAtlasPath = SDL_GetBasePath() + atlasPath;
    impala::log::Info(impala::log::LogCategory::Assets, "Adding sprite atlas '{}'", atlasPath);
    auto atlas = std::make_unique<SpriteAtlas>(atlasPath, fullAtlasPath);
    atlas->Initialize();
    _spriteAtlases[atlasPath] = std::move(atlas);
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

SpriteHandle AssetDatabase::AddSprite(const std::string& name, const std::string& spritePath) {
    auto it = _spritesHandleMap.find(name);

    if (it != _spritesHandleMap.end()) {
        printf("Sprite '%s' already exists\n", name.c_str());
        return it->second;
    }

    auto fullSpritePath = SDL_GetBasePath() + spritePath;
    impala::log::Info(impala::log::LogCategory::Assets, "Adding sprite '{}' - '{}'", name,
                      fullSpritePath);
    auto sprite = std::make_unique<Sprite>(name, fullSpritePath);

    auto returnSprite = static_cast<SpriteHandle>(_sprites.size());
    _sprites.push_back(std::move(sprite));
    _spritesHandleMap[name] = returnSprite;
    return returnSprite;
}

SpriteHandle AssetDatabase::GetSprite(const std::string& name) {
    auto it = _spritesHandleMap.find(name);
    if (it != _spritesHandleMap.end()) {
        return it->second;
    }

    impala::log::Error(impala::log::LogCategory::Assets, "Sprite '{}' not found", name);
    return InvalidSpriteHandle();
}

SpriteHandle AssetDatabase::GetSprite(const std::string& spriteAtlas, const std::string& name) {
    auto atlasIt = _spriteAtlases.find(spriteAtlas);
    if (atlasIt != _spriteAtlases.end()) {
        return atlasIt->second->GetSprite(name);
    }

    impala::log::Error(impala::log::LogCategory::Assets, "Sprite atlas '{}' not found",
                       spriteAtlas);
    return InvalidSpriteHandle();
}