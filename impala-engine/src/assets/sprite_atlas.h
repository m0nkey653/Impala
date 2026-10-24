#pragma once

#include "impala/sprite.h"

namespace impala {
    struct SpriteRect {
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
    };

    typedef std::unordered_map<std::string, SpriteRect> SpriteRectMap;

    class SpriteAtlas : public impala::ISpriteAtlas {
    public:
        SpriteAtlas(const std::string& name, const std::string& atlasPath);
        void Initialize();

        std::string Name() override;
        void Load() override;

        impala::ISprite* GetSprite(const std::string& name) override;

    private:
        std::string _name;
        std::string _atlasPath;
        bool _initialized = false;
        bool _loaded = false;

        // populated during initialization
        std::string _sheetPath;
        SpriteRectMap _spriteRects;
    };

} // namespace impala