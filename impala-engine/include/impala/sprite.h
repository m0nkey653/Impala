#pragma once

#include "impala/asset.h"
#include "impala/handles.h"
#include <string>

namespace impala {

    class ISprite : public IAsset {
    public:
        virtual ~ISprite() = default;
    };

    class ISpriteAtlas : public IAsset {
    public:
        virtual ~ISpriteAtlas() = default;
        virtual SpriteHandle GetSprite(const std::string& name) = 0;
    };

    struct SpriteRenderer {
        SpriteHandle sprite;
        uint32_t color = 0xFFFFFFFF;
        uint32_t layer = 0;
    };

} // namespace impala
