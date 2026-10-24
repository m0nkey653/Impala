#pragma once

#include "impala/sprite.h"
#include <string>

namespace impala {

    class IAssetDataBase {
    public:
        virtual ~IAssetDataBase() = default;
        virtual void AddSpriteAtlas(const std::string& name, const std::string& atlasPath) = 0;
        virtual ISpriteAtlas* GetSpriteAtlas(const std::string& name) = 0;
    };

} // namespace impala