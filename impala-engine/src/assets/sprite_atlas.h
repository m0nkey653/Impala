#pragma once

#include "impala/sprite.h"

namespace impala {

    class SpriteAtlas : public impala::ISpriteAtlas {
    public:
        SpriteAtlas(const std::string& name, const std::string& atlasPath);
        void Initialize();

        std::string Name() override;
        void Load() override;

        SpriteHandle GetSprite(const std::string& name) override;

        // Internal method to get the sprite rect from a sprite handle
        ImpalaSprite GetSprite(SpriteHandle spriteHandle);
        bool IsLoaded() const { return _initialized && _sheet != nullptr; }

    private:
        std::string _name;
        std::string _atlasPath;
        bool _initialized = false;

        // populated during initialization
        std::string _sheetPath;
        std::unordered_map<std::string, SpriteRect> _spriteRects;

        SDL_Surface* _sheet = nullptr;
    };

} // namespace impala