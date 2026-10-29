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
        Texture(std::string path) : _path(std::move(path)) {}
        ~Texture() = default;

        void Load(Renderer* renderer) {
            auto surface = SDL_LoadSurface(_path.c_str());
            if (!surface) {
                throw std::runtime_error("Failed to load surface from path: " + _path);
            }

            _texture = std::unique_ptr<SDL_Texture, detail::TextureDeleter>(
                SDL_CreateTextureFromSurface(renderer->Get(), surface));

            SDL_DestroySurface(surface);
        };
        bool IsLoaded() const { return _texture != nullptr; }
        SDL_Texture* Get() const { return _texture.get(); }

    private:
        std::unique_ptr<SDL_Texture, detail::TextureDeleter> _texture = nullptr;
        std::string _path;
        ;
    };
} // namespace impala