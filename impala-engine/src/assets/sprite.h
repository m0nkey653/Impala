#pragma once

#include "../surface.h"
#include "impala/sprite.h"

namespace impala {

    class Sprite : public impala::ISprite {
    public:
        std::string Name() override;
        void Load() override;

    private:
        std::string _name;
        std::string _spritePath;
        std::unique_ptr<Surface> _surface;
    };

} // namespace impala