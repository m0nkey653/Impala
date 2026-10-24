#pragma once

#include "impala/asset_database.h"
#include "impala/sprite.h"
#include <memory>
#include <unordered_map>

namespace impala {
    class Sprite : public impala::ISprite {
    public:
        std::string Name() override;
        void Load() override;

    private:
        std::string _name;
        std::string _spritePath;
    };

    class AssetDatabase : public impala::IAssetDataBase {
    public:
        impala::ISpriteAtlas* GetSpriteAtlas(const std::string& name) override;
        void AddSpriteAtlas(const std::string& name, const std::string& atlasPath) override;

    private:
        std::unordered_map<std::string, std::unique_ptr<impala::ISpriteAtlas>> _spriteAtlases;
    };

} // namespace impala