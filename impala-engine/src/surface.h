#pragma once

namespace impala {

    struct SurfaceDeleter {
        void operator()(SDL_Surface* surface) const { SDL_DestroySurface(surface); }
    };

    class Surface {
    public:
        Surface() = default;
        ~Surface() = default;

        void Load(const std::string& path) {
            auto surface = SDL_LoadSurface(path.c_str());
            if (!surface) {
                throw std::runtime_error("Failed to load surface from path: " + path);
            }
            _surface.reset(surface);
        }
        bool IsLoaded() const { return _surface != nullptr; }
        SDL_Surface* Get() const;

    private:
        std::unique_ptr<SDL_Surface, SurfaceDeleter> _surface = nullptr;
    };

} // namespace impala