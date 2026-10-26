#pragma once

#include "impala/asset.h"
#include <string>

namespace impala {

    struct SpriteHandle {
        uint32_t id = 0;
        constexpr bool IsValid() const { return id != UINT32_MAX; }
    };

    constexpr SpriteHandle InvalidSpriteHandle() {
        return SpriteHandle{UINT32_MAX};
    }

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
