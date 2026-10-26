#pragma once
#include "renderer.h"
#include "surface.h"

namespace impala {
    class Texture {
    public:
        Texture() = default;
        ~Texture();

        void Load(Renderer* renderer, Surface* surface) {
            _texture = std::unique_ptr<SDL_Texture>(
                SDL_CreateTextureFromSurface(renderer->Get(), surface->Get()));
        };
        bool IsLoaded() const { return _texture != nullptr; }
        SDL_Texture* Get() const { return _texture.get(); }
        void Free();

    private:
        std::unique_ptr<SDL_Texture> _texture = nullptr;
    };
} // namespace impala