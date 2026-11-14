#pragma once
#include "impala/handles.h"
#include "impala/math.h"
#include "sprite_rect.h"

namespace impala {

    constexpr impala::math::Vec2 ANCHOR_LEFT = {-1, 0};
    constexpr impala::math::Vec2 ANCHOR_CENTER = {0, 0};
    constexpr impala::math::Vec2 ANCHOR_RIGHT = {1, 0};
    constexpr impala::math::Vec2 ANCHOR_TOP = {0, -1};
    constexpr impala::math::Vec2 ANCHOR_BOTTOM = {0, 1};
    constexpr impala::math::Vec2 ANCHOR_TOP_LEFT = {-1, -1};
    constexpr impala::math::Vec2 ANCHOR_TOP_RIGHT = {1, -1};
    constexpr impala::math::Vec2 ANCHOR_BOTTOM_LEFT = {-1, 1};
    constexpr impala::math::Vec2 ANCHOR_BOTTOM_RIGHT = {1, 1};

    // constexpr impala::math::Vec2 PositionFromAnchor(const impala::math::Vec2& position,
    //                                                 const impala::math::Vec2& anchor,
    //                                                 const impala::math::Vec2& size) {
    //     return {position.x + anchor.x * size.x * 0.5f, position.y + anchor.y * size.y * 0.5f};
    // }

    // transform 0,0
    // anchor 0,0 is the center of the sprite
    // sprite is size 2, 2
    // center the sprite on 1,1
    // which means rendering at -1,-1

    // transform 1,1
    // anchor 0,0 is the center of the sprite
    // sprite is size 4, 4
    // center the sprite on 1,1
    // which means rendering at -1,-1

    constexpr impala::math::Vec2 PositionFromAnchor(const impala::math::Vec2& position,
                                                    const impala::math::Vec2& anchor,
                                                    const impala::math::Vec2& size) {
        // get position offset based on anchor and size
        // this calculation below doesn't work if anchor is 0,0 because it assumes anchor is
        // relative to the sprite's center so instead, apply the anchor to the position to get the
        // direction, then offset by half the size in that direction

        // x = 1
        // x_size = 4
        // x_anchor = 0
        // x_offset = x - x_size * 0.5 - x_anchor * x_size * 0.5f
        // x_offset = 1 - 4 * 0.5 - 0 * 4 * 0.5f
        // x_offset = 1 - 2 - 0
        // x_offset = -1

        // x = 1
        // x_size = 4
        // x_anchor = -1
        // x_offset = x - x_size * 0.5 - x_anchor * x_size * 0.5f
        // x_offset = 1 - 4 * 0.5 - (-1) * 4 * 0.5f
        // x_offset = 1 - 2 - (-2)
        // x_offset = 1 - 2 + 2
        // x_offset = 1
        auto x = position.x - size.x * -(anchor.x * size.x * 0.5f);
        auto y = position.y - size.y * -(anchor.y * size.y * 0.5f);
        return {x, y};
    }

    struct ImpalaSprite {
        SpriteRect rect;
        TextureHandle textureHandle;
        impala::math::Vec2 anchor = ANCHOR_CENTER;
    };

    constexpr ImpalaSprite InvalidSprite() {
        return {{0, 0, 0, 0}, TextureHandle(), {0, 0}};
    }

} // namespace impala
