#pragma once

#include "impala/asset.h"
#include "impala/handles.h"
#include <string>

namespace impala {

    class ISpriteAtlas : public IAsset {
    public:
        virtual ~ISpriteAtlas() = default;
        virtual SpriteHandle GetSprite(const std::string& name) = 0;
    };

    struct SpriteRenderer {
        SpriteHandle sprite;
        uint8_t colorR = 0xff;
        uint8_t colorG = 0xff;
        uint8_t colorB = 0xff;
        uint8_t colorA = 0xff;
        uint32_t layer = 0;

        bool IsDefaultColor() const {
            return colorR == 0xff && colorG == 0xff && colorB == 0xff && colorA == 0xff;
        }
    };

} // namespace impala
