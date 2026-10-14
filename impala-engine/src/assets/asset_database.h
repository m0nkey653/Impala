#pragma once
#include <tuple>
#include <unordered_map>

#include "impala/game.h"

class SpriteAtlas : public impala::IAsset {
public:
    std::string Name() override;
    void Load() override;

private:
    std::unordered_map<std::string, std::tuple<int, int, int, int>> _spriteRects;
    std::string _name;
    std::string _atlasPath;
};

class AssetDatabase : public impala::IAssetDataBase {
public:
    void AddSpriteAtlas(const std::string& name, const std::string& atlasPath) override;

private:
};
