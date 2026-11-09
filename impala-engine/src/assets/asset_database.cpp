#include "asset_database.h"
#include "../impala_sprite.h"
#include "../texture.h"
#include "impala/log.h"
#include "sprite_atlas.h"

using namespace impala;

void AssetDatabase::AddSpriteAtlas(const std::string& atlasPath) {
    auto it = _spriteAtlases.find(atlasPath);

    if (it != _spriteAtlases.end()) {
        impala::log::Debug(impala::log::LogCategory::Assets, "Sprite atlas '{}' already exists",
                           atlasPath);
    }

    auto fullAtlasPath = SDL_GetBasePath() + atlasPath;
    impala::log::Info(impala::log::LogCategory::Assets, "Adding sprite atlas '{}'", atlasPath);
    auto atlas = std::make_unique<SpriteAtlas>(atlasPath, fullAtlasPath);
    atlas->Initialize();

    impala::log::Info(impala::log::LogCategory::Assets, "Sprite atlas '{}' added successfully",
                      atlasPath);
    auto sheetPath = atlas->GetSheetPath();

    auto textureHandle = AddTexture(sheetPath);
    auto atlasFolder = atlasPath.substr(0, atlasPath.find_last_of("/")) + "/";

    // we're fast-loading them all on parse, that gives us extra sprite entries on load
    // we might as well while we're doing XML stuff
    // associate each sprite rect with the texture handle
    for (const auto& pair : atlas->GetSpriteRects()) {

        auto spriteName = pair.first;
        auto spriteRect = pair.second;

        std::string spriteKey = atlasFolder + pair.first;

        impala::log::Debug(impala::log::LogCategory::Assets,
                           "Adding sprite '{}' with rect ({}, {}, {}, {})", spriteName,
                           spriteRect.x, spriteRect.y, spriteRect.width, spriteRect.height);

        auto spriteHandle = AddSprite(spriteKey, spriteRect, textureHandle);
    }

    // we fast-loaded all the sprites - is this any use?
    _spriteAtlases[atlasPath] = std::move(atlas);
}

TextureHandle AssetDatabase::AddTexture(const std::string& texturePath) {
    auto it = _texturesHandleMap.find(texturePath);
    if (it != _texturesHandleMap.end()) {
        // impala::log::Debug(impala::log::LogCategory::Assets, "Texture add called for existing
        // path '{}'", texturePath);
        return it->second;
    }

    auto texture = std::make_unique<impala::Texture>(texturePath);
    _textures.push_back(std::move(texture));

    // Add the texture
    TextureHandle t = static_cast<TextureHandle>(_textures.size() - 1);
    _texturesHandleMap[texturePath] = t;
    impala::log::Debug(impala::log::LogCategory::Assets, "Texture added '{}'", texturePath);

    return t;
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

SpriteHandle AssetDatabase::AddSprite(const std::string& name, TextureHandle textureHandle) {

    auto it = _spritesHandleMap.find(name);
    if (it != _spritesHandleMap.end()) {
        // impala::log::Debug(impala::log::LogCategory::Assets, "Sprite add for existing name '{}'",
        //                    name);
        // do more here? compare texture handles?
        return it->second;
    }

    TextureHandle t = textureHandle;
    if (t.IsValid()) {
        if (t.id >= _textures.size()) {
            // invalid texture handle - greater than collection
            t = InvalidTextureHandle();
        } else if (t.generation != _textures[t.id]->GetGeneration()) {
            // invalid texture - handle expired
            t = InvalidTextureHandle();
        }
    }

    // fill with zero for now
    // we can populate the rect on load
    ImpalaSprite sprite = {{0, 0, 0, 0, 0}, t};
    _sprites.push_back(std::move(sprite));
    auto returnSprite = static_cast<SpriteHandle>(_sprites.size());
    _spritesHandleMap[name] = returnSprite;
    return returnSprite;
}

SpriteHandle AssetDatabase::AddSprite(const std::string& name, SpriteRect rect,
                                      TextureHandle textureHandle) {

    auto it = _spritesHandleMap.find(name);
    if (it != _spritesHandleMap.end()) {
        // impala::log::Debug(impala::log::LogCategory::Assets, "Sprite add for existing name '{}'",
        //                    name);
        // do more here? compare texture handles?
        return it->second;
    }

    TextureHandle t = textureHandle;
    if (t.IsValid()) {
        if (t.id >= _textures.size()) {
            // invalid texture handle - greater than collection
            t = InvalidTextureHandle();
        } else if (t.generation != _textures[t.id]->GetGeneration()) {
            // invalid texture - handle expired
            t = InvalidTextureHandle();
        }
    }

    // fill with zero for now
    // we can populate the rect on load
    ImpalaSprite sprite = {rect, t};
    _sprites.push_back(std::move(sprite));
    auto returnSprite = static_cast<SpriteHandle>(_sprites.size() - 1);
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

ImpalaSprite AssetDatabase::GetSprite(SpriteHandle handle) {
    if (!handle.IsValid()) {
        return InvalidSprite();
    }
    if (handle.id >= 0 && handle.id < static_cast<int>(_sprites.size())) {
        return _sprites[handle.id];
    }
    return InvalidSprite();
}

Texture* AssetDatabase::GetTexture(TextureHandle handle) {
    if (!handle.IsValid()) {
        return nullptr;
    }
    if (handle.id >= 0 && handle.id < static_cast<int>(_textures.size())) {
        return _textures[handle.id].get();
    }
    return nullptr;
}

TextureHandle AssetDatabase::LoadSpriteTexture(SpriteHandle handle) {
    auto sprite = GetSprite(handle);

    // auto texture = std::make_unique<Texture>(sprite->GetPath());
    auto texture = GetTexture(sprite.textureHandle);
    // we have to check generation here.
    if (!texture) {
        impala::log::Error(impala::log::LogCategory::Assets,
                           "Texture for sprite handle '{}' not found", handle.id);
        return InvalidTextureHandle();
    }

    if (texture->IsLoaded()) {
        impala::log::Info(impala::log::LogCategory::Assets,
                          "Texture for sprite handle '{}' is already loaded", handle.id);
        return sprite.textureHandle;
    }

    try {
        texture->Load(_renderer.get());
        impala::log::Info(impala::log::LogCategory::Assets, "Loaded texture [{}] for sprite [{}]",
                          sprite.textureHandle.id, handle.id);

    } catch (const std::exception e) {
        impala::log::Error(impala::log::LogCategory::Assets,
                           "Texture for sprite handle '{}' not found", handle.id);
        return InvalidTextureHandle();
    }

    return sprite.textureHandle;
}