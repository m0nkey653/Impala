#pragma once
#include "impala/color.h"
#include "impala/math.h"

namespace impala {

    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        virtual void DrawLine(impala::math::Vec2 start, impala::math::Vec2 end,
                              impala::Color color) = 0;
        // Draws a circle with the given position, radius, and color.
        // 0 = top of the screan and rotates clockwise
        virtual void DrawCircle(impala::math::Vec2 position, float radius, impala::Color color,
                                float startAngle = 0.0f, float endAngle = 360.0f) = 0;
        virtual void DrawRectangle(impala::math::Vec2 topLeft, float width, float height,
                                   impala::Color color) = 0;
        virtual void FillRectangle(impala::math::Vec2 topLeft, float width, float height,
                                   impala::Color color) = 0;
    };

} // namespace impala
