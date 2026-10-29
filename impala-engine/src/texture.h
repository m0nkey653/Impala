#pragma once
#include "renderer.h"
#include "surface.h"

namespace impala {

    namespace detail {
        struct TextureDeleter {
            void operator()(SDL_Texture* texture) const { SDL_DestroyTexture(texture); }
        };
    } // namespace detail

    class Texture {
    public:
        Texture() = default;
        ~Texture() = default;

        void Load(Renderer* renderer, Surface* surface) {
            _texture = std::unique_ptr<SDL_Texture, detail::TextureDeleter>(
                SDL_CreateTextureFromSurface(renderer->Get(), surface->Get()));
        };
        bool IsLoaded() const { return _texture != nullptr; }
        SDL_Texture* Get() const { return _texture.get(); }

    private:
        std::unique_ptr<SDL_Texture, detail::TextureDeleter> _texture = nullptr;
    };
} // namespace impala