#include "sprite_atlas.h"
#include "impala/errors.h"
#include <pugixml.hpp>

/*
Sparrow/Starling atlas format
This format is used by the Sparrow and Starling frameworks for defining sprite atlases.
Example:
<TextureAtlas imagePath="sheet.png">
        <SubTexture name="beam0.png" x="143" y="377" width="43" height="31"/>
        <SubTexture name="beam1.png" x="327" y="644" width="40" height="20"/>
</TextureAtlas>
*/

SpriteAtlas::SpriteAtlas(const std::string& name, const std::string& atlasPath)
    : _name(name), _atlasPath(atlasPath) {}

std::string SpriteAtlas::Name() {
    return _name;
}

void SpriteAtlas::Initialize() {
    // parse atlas file
    // populate dictionary with name <x, y, width, height> mappings

    pugi::xml_document doc;
    if (!doc.load_file(_atlasPath.c_str())) {
        throw impala::AssetError("Failed to load atlas file: " + _atlasPath);
    }
    pugi::xml_node xmlTextureAtlas = doc.child("TextureAtlas");
    if (!xmlTextureAtlas) {
        throw impala::AssetError("Failed to find TextureAtlas node in atlas file: " + _atlasPath);
    }
    std::string sheetPath = xmlTextureAtlas.attribute("imagePath").as_string();
    if (sheetPath.empty()) {
        throw impala::AssetError("Failed to find imagePath attribute in TextureAtlas node: " +
                                 _atlasPath);
    }

    for (pugi::xml_node subTexture : xmlTextureAtlas.children("SubTexture")) {
        if (!subTexture) {
            SDL_Log("Missing SubTexture node in atlas file: %s", _atlasPath.c_str());
            break;
        }
        if (!subTexture.attribute("name")) {
            SDL_Log("Missing name attribute in SubTexture node in atlas file: %s",
                    _atlasPath.c_str());
            continue;
        }

        auto spriteNameNode = subTexture.attribute("name");
        auto xNode = subTexture.attribute("x");
        auto yNode = subTexture.attribute("y");
        auto widthNode = subTexture.attribute("width");
        auto heightNode = subTexture.attribute("height");

        if (!spriteNameNode || !xNode || !yNode || !widthNode || !heightNode) {
            SDL_Log("Missing one or more attributes in SubTexture node in atlas file: %s",
                    _atlasPath.c_str());
            continue;
        }

        std::string spriteName = spriteNameNode.as_string();
        uint32_t x = xNode.as_uint();
        uint32_t y = yNode.as_uint();
        uint32_t width = widthNode.as_uint();
        uint32_t height = heightNode.as_uint();

        if (_spriteRects.find(spriteName) == _spriteRects.end()) {
            _spriteRects[spriteName] = SpriteRect{x, y, width, height};
            // TODO: Add trace logging for new sprite rects
            SDL_Log("Added new sprite rect for '%s' in atlas file: %s", spriteName.c_str(),
                    _atlasPath.c_str());
        } else {
            // handle duplicate sprite names if necessary
            // TODO: Add warning logging
        }
    }

    _sheetPath = sheetPath;
    _initialized = true;
}

void SpriteAtlas::Load() {
    if (!_initialized || _sheetPath.empty()) {
        throw std::runtime_error("SpriteAtlas not successfully initialized: " + _atlasPath);
    }

    SDL_Surface* sheet = SDL_LoadSurface(_sheetPath.c_str());
    if (!sheet) {
        SDL_Log("Failed to load surface '%s': %s", _sheetPath.c_str(), SDL_GetError());
    }
}