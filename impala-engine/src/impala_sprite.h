#pragma once
#include "impala/handles.h"
#include "impala/math.h"
#include "sprite_rect.h"

namespace impala {

    constexpr impala::math::Vec2 PIVOT_LEFT = {-1, 0};
    constexpr impala::math::Vec2 PIVOT_CENTER = {0, 0};
    constexpr impala::math::Vec2 PIVOT_RIGHT = {1, 0};
    constexpr impala::math::Vec2 PIVOT_TOP = {0, -1};
    constexpr impala::math::Vec2 PIVOT_BOTTOM = {0, 1};
    constexpr impala::math::Vec2 PIVOT_TOP_LEFT = {-1, -1};
    constexpr impala::math::Vec2 PIVOT_TOP_RIGHT = {1, -1};
    constexpr impala::math::Vec2 PIVOT_BOTTOM_LEFT = {-1, 1};
    constexpr impala::math::Vec2 PIVOT_BOTTOM_RIGHT = {1, 1};

    // Calculates the position of a sprite based on its anchor and size.
    // 0,0
    constexpr impala::math::Vec2 PositionFromPivot(const impala::math::Vec2& position,
                                                   const impala::math::Vec2& pivot,
                                                   const impala::math::Vec2& size) {
        // get position offset based on anchor and size
        // this calculation below doesn't work if pivot is 0,0 because it assumes pivot is
        // relative to the sprite's center so instead, apply the pivot to the position to get the
        // direction, then offset by half the size in that direction
        auto x = position.x - size.x * -(pivot.x * size.x * 0.5f);
        auto y = position.y - size.y * -(pivot.y * size.y * 0.5f);
        return {x, y};
    }

    struct ImpalaSprite {
        SpriteRect rect;
        TextureHandle textureHandle;
        impala::math::Vec2 pivot = PIVOT_CENTER;
    };

    constexpr ImpalaSprite InvalidSprite() {
        return {{0, 0, 0, 0}, TextureHandle(), {0, 0}};
    }

} // namespace impala
