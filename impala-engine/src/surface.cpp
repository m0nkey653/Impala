#include "surface.h"

namespace impala {
    Surface::~Surface() {
        Free();
    }

    void Surface::Load(const std::string& path) {
        auto surface = SDL_LoadSurface(path.c_str());
        if (!surface) {
            throw std::runtime_error("Failed to load surface from path: " + path);
        }
        _surface.reset(surface);
    }

    void Surface::Free() {
        if (_surface) {
            SDL_DestroySurface(_surface.release());
        }
        _surface = nullptr;
    }
} // namespace impala