#pragma once

namespace impala {
    namespace detail {
        struct RendererDeleter {
            void operator()(SDL_Renderer* renderer) const { SDL_DestroyRenderer(renderer); }
        };
    } // namespace detail

    class Renderer {
    public:
        Renderer(SDL_Renderer* renderer) : _renderer(renderer) {}
        ~Renderer() = default;
        SDL_Renderer* Get() const { return _renderer.get(); }

    private:
        std::unique_ptr<SDL_Renderer, detail::RendererDeleter> _renderer = nullptr;
    };
} // namespace impala
