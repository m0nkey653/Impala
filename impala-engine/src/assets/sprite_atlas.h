#pragma once

#include "impala/game.h"
#include <string>
class SpriteAtlas : public impala::IAsset {
public:
    SpriteAtlas(const std::string& name, const std::string& sheetPath,
                const std::string& atlasPath);

    std::string Name() override;
    void Load() override;

private:
    std::string _name;
    std::string _sheetPath;
    std::string _atlasPath;
};