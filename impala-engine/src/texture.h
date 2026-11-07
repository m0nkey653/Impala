#pragma once
#include "renderer.h"

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
            std::string surfacePath = SDL_GetBasePath();
            surfacePath.append("/");
            surfacePath.append(_path);

            auto surface = SDL_LoadSurface(surfacePath.c_str());
            if (!surface) {
                auto e = SDL_GetError();
                throw std::runtime_error("Failed to load surface from path: " + _path + " - " + e);
            }

            auto t = SDL_CreateTextureFromSurface(renderer->Get(), surface);
            SDL_DestroySurface(surface);
            if (!t) {
                auto e = SDL_GetError();
                throw std::runtime_error("Failed to load texture from surface: " + _path + " - " +
                                         e);
            }

            _texture = std::unique_ptr<SDL_Texture, detail::TextureDeleter>(t);
        };
        bool IsLoaded() const { return _texture != nullptr; }
        SDL_Texture* GetTexture() const { return _texture.get(); }
        uint32_t GetGeneration() const { return _generation; }

    private:
        std::unique_ptr<SDL_Texture, detail::TextureDeleter> _texture = nullptr;
        std::string _path;
        uint32_t _generation;
    };
} // namespace impala