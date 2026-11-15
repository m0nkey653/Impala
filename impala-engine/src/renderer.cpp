#include "renderer.h"

namespace impala {

    void Renderer::DrawLine(impala::math::Vec2 start, impala::math::Vec2 end, impala::Color color) {
        SDL_SetRenderDrawColorFloat(_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_RenderLine(_renderer.get(), start.x, start.y, end.x, end.y);
    }

    void Renderer::DrawCircle(impala::math::Vec2 position, float radius, impala::Color color,
                              float startAngle, float endAngle) {
        const int segments = 60;
        SDL_SetRenderDrawColorFloat(_renderer.get(), color.r, color.g, color.b, color.a);

        SDL_FRect rects[segments];
        math::Vec2 point{0, -1};
        math::Vec2 prev = point;
        for (int i = 1; i <= segments; i++) {
            float angle = startAngle + (endAngle - startAngle) * (i / static_cast<float>(segments));
            impala::math::Vec2 current = math::RotateDegrees(point, angle);
            rects[i - 1] = {position.x + current.x * radius - 1,
                            position.y + current.y * radius - 1, 2, 2};
            prev = current;
        }
        SDL_RenderFillRects(_renderer.get(), rects, segments);
    }

    void Renderer::DrawRectangle(impala::math::Vec2 topLeft, float width, float height,
                                 impala::Color color) {
        SDL_SetRenderDrawColorFloat(_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_FRect rect = {topLeft.x, topLeft.y, width, height};
        SDL_RenderRect(_renderer.get(), &rect);
    }

    void Renderer::FillRectangle(impala::math::Vec2 topLeft, float width, float height,
                                 impala::Color color) {
        SDL_SetRenderDrawColorFloat(_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_FRect rect = {topLeft.x, topLeft.y, width, height};
        SDL_RenderFillRect(_renderer.get(), &rect);
    }

} // namespace impala
