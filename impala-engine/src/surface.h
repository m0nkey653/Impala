#pragma once

namespace impala {

    class Surface {
    public:
        Surface() = default;
        ~Surface();

        void Load(const std::string& path);
        void Free();
        bool IsLoaded() const { return _surface != nullptr; }

        SDL_Surface* Get() const;

    private:
        std::unique_ptr<SDL_Surface> _surface = nullptr;
    };

} // namespace impala