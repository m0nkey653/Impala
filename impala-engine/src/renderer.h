#pragma once
#include "impala/renderer.h"

namespace impala {
    namespace detail {
        struct RendererDeleter {
            void operator()(SDL_Renderer* renderer) const { SDL_DestroyRenderer(renderer); }
        };
    } // namespace detail

    class Renderer : public IRenderer {
    public:
        Renderer(SDL_Renderer* renderer) : _renderer(renderer) {}
        ~Renderer() = default;
        SDL_Renderer* Get() const { return _renderer.get(); }

        void DrawLine(impala::math::Vec2 start, impala::math::Vec2 end,
                      impala::Color color) override;
        void DrawCircle(impala::math::Vec2 position, float radius, impala::Color color) override;
        void DrawRectangle(impala::math::Vec2 topLeft, float width, float height,
                           impala::Color color) override;
        void FillRectangle(impala::math::Vec2 topLeft, float width, float height,
                           impala::Color color) override;

    private:
        std::unique_ptr<SDL_Renderer, detail::RendererDeleter> _renderer = nullptr;
    };
} // namespace impala
