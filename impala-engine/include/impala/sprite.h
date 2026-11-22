#pragma once

#include "impala/asset.h"
#include "impala/color.h"
#include "impala/handles.h"
#include "impala/math.h"
#include <optional>
#include <string>

namespace impala {

    class ISpriteAtlas : public IAsset {
    public:
        virtual ~ISpriteAtlas() = default;
        virtual SpriteHandle GetSprite(const std::string& name) = 0;
    };

    struct SpriteRenderer {
        SpriteHandle sprite;
        Color color = color::White();
        uint32_t layer = 0;
        std::optional<impala::math::Vec2> anchor;

        bool IsDefaultColor() const { return color == color::White(); }
    };

} // namespace impala
