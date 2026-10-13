#pragma once

#include "impala/game.h"

class AssetDatabase : public impala::IAssetDataBase {
public:
    void AddSpriteAtlas(const std::string& name, const std::string& sheetPath,
                        const std::string& atlasPath) override;

private:
};