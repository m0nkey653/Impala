#pragma once

#include "impala/asset_database.h"
#include <tuple>
#include <unordered_map>

class Sprite : public impala::ISprite {
public:
    std::string Name() override;
    void Load() override;

private:
    std::string _name;
    std::string _spritePath;
};

class SpriteAtlas : public impala::ISpriteAtlas {
public:
    std::string Name() override;
    void Load() override;
    impala::ISprite* GetSprite(const std::string& name) override;

private:
    std::unordered_map<std::string, std::tuple<int, int, int, int>> _spriteRects;
    std::string _name;
    std::string _atlasPath;
};

class AssetDatabase : public impala::IAssetDataBase {
public:
    SpriteAtlas* GetSpriteAtlas(const std::string& name) override;
    void AddSpriteAtlas(const std::string& name, const std::string& atlasPath) override;

private:
    std::unordered_map<std::string, SpriteAtlas> _spriteAtlases;
};
