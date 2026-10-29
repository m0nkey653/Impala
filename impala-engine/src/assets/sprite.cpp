#include "sprite.h"

using namespace impala;

std::string Sprite::Name() {
    return _name;
}

Sprite::Sprite(const std::string& name, const std::string& spritePath)
    : _name(name), _spritePath(spritePath), _texture(InvalidTextureHandle()) {}
