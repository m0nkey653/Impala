#pragma once

#include "impala/assets.h"
#include <string>

namespace impala {

    struct SpriteHandle {
        uint32_t id = 0;
    };

    class ISprite : public IAsset {
    public:
        virtual ~ISprite() = default;
    };

    class ISpriteAtlas : public IAsset {
    public:
        virtual ~ISpriteAtlas() = default;
        virtual ISprite* GetSprite(const std::string& name) = 0;
    };

    struct SpriteRenderer {
        SpriteHandle sprite;
        uint32_t color = 0xFFFFFFFF;
        uint32_t layer = 0;
    };

} // namespace impala
