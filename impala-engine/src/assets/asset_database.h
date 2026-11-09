#pragma once

#include "../renderer.h"
#include "../texture.h"
#include "impala/asset_database.h"
#include "impala/sprite.h"
#include "sprite.h"
#include "sprite_atlas.h"

namespace impala {

    class AssetDatabase : public impala::IAssetDataBase {
    public:
        AssetDatabase(Renderer* renderer) : _renderer(renderer) {}
        void AddSpriteAtlas(const std::string& atlasPath) override;

        SpriteHandle AddSprite(const std::string& name, TextureHandle textureHandle) override;
        TextureHandle AddTexture(const std::string& texturePath) override;

        SpriteHandle GetSprite(const std::string& name) override;
        SpriteHandle GetSprite(const std::string& spriteAtlas, const std::string& name) override;

        impala::TextureHandle LoadSpriteTexture(SpriteHandle handle) override;

        impala::ISpriteAtlas* GetSpriteAtlas(const std::string& name);
        impala::ImpalaSprite GetSprite(SpriteHandle handle);
        impala::Texture* GetTexture(TextureHandle handle);

        SpriteHandle AddSprite(const std::string& name, SpriteRect rect,
                               TextureHandle textureHandle);

    private:
        std::unordered_map<std::string, std::unique_ptr<impala::SpriteAtlas>> _spriteAtlases;

        // mapped to relative path of the texture file
        std::unordered_map<std::string, TextureHandle> _texturesHandleMap;
        std::unordered_map<std::string, SpriteHandle> _spritesHandleMap;

        std::vector<impala::ImpalaSprite> _sprites;
        std::vector<std::unique_ptr<impala::Texture>> _textures;

    private:
        std::unique_ptr<impala::Renderer> _renderer;
    };

} // namespace impala