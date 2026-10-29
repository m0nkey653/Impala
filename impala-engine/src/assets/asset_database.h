#pragma once

#include "../texture.h"
#include "impala/asset_database.h"
#include "impala/sprite.h"
#include "sprite.h"
#include "sprite_atlas.h"

namespace impala {

    class AssetDatabase : public impala::IAssetDataBase {
    public:
        void AddSpriteAtlas(const std::string& atlasPath) override;

        SpriteHandle AddSprite(const std::string& name, const std::string& spritePath) override;

        SpriteHandle GetSprite(const std::string& name) override;
        SpriteHandle GetSprite(const std::string& spriteAtlas, const std::string& name) override;
        TextureHandle GetTexture(const std::string& name) override;

        impala::ISpriteAtlas* GetSpriteAtlas(const std::string& name);
        impala::Sprite* GetSprite(SpriteHandle handle);

    private:
        std::unordered_map<std::string, std::unique_ptr<impala::SpriteAtlas>> _spriteAtlases;
        std::unordered_map<std::string, TextureHandle> _texturesHandleMap;
        std::unordered_map<std::string, SpriteHandle> _spritesAtlasHandleMap;
        std::unordered_map<std::string, SpriteHandle> _spritesHandleMap;

        std::vector<std::unique_ptr<impala::Sprite>> _sprites;
        std::vector<std::unique_ptr<impala::Texture>> _textures;
    };

} // namespace impala