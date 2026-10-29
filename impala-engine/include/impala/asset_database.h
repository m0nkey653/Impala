#pragma once

#include "impala/handles.h"
#include <string>

namespace impala {

    class IAssetDataBase {
    public:
        virtual ~IAssetDataBase() = default;

        virtual void AddSpriteAtlas(const std::string& atlasPath) = 0;

        virtual SpriteHandle AddSprite(const std::string& name, const std::string& spritePath) = 0;
        virtual SpriteHandle GetSprite(const std::string& name) = 0;
        virtual SpriteHandle GetSprite(const std::string& spriteAtlas, const std::string& name) = 0;

        virtual impala::TextureHandle LoadSpriteTexture(SpriteHandle handle) = 0;
    };

} // namespace impala