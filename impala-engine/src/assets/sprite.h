#pragma once

#include "impala/handles.h"
#include "impala/sprite.h"

namespace impala {

    class Sprite : public impala::ISprite {
    public:
        Sprite(const std::string& name, const std::string& spritePath);
        std::string Name() override;

    private:
        std::string _name;
        std::string _spritePath;
        impala::TextureHandle _texture;
    };

} // namespace impala