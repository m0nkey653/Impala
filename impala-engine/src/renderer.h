#pragma once

namespace impala {
    class Renderer {
    public:
        Renderer(std::unique_ptr<SDL_Renderer> renderer) : _renderer(std::move(renderer)) {}
        ~Renderer();

        void Free() { SDL_DestroyRenderer(_renderer.release()); }
        SDL_Renderer* Get() const { return _renderer.get(); }

    private:
        std::unique_ptr<SDL_Renderer> _renderer = nullptr;
    }
} // namespace impala