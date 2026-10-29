#include "sprite_atlas.h"
#include "../impala_sprite.h"
#include "../texture.h"
#include "impala/error.h"
#include "impala/log.h"
#include <pugixml.hpp>

using namespace impala;
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
        throw impala::AssetError("Failed to load atlas file: '" + _atlasPath + "'");
    }
    pugi::xml_node xmlTextureAtlas = doc.child("TextureAtlas");
    if (!xmlTextureAtlas) {
        throw impala::AssetError("Failed to find TextureAtlas node in atlas file: '" + _atlasPath +
                                 "'");
    }
    std::string sheetPath = xmlTextureAtlas.attribute("imagePath").as_string();
    if (sheetPath.empty()) {
        throw impala::AssetError("Failed to find imagePath attribute in TextureAtlas node: '" +
                                 _atlasPath + "'");
    }

    for (pugi::xml_node subTexture : xmlTextureAtlas.children("SubTexture")) {
        if (!subTexture) {
            impala::log::Debug(impala::log::LogCategory::Assets,
                               "Missing SubTexture node in atlas file: {}", _atlasPath);
            break;
        }
        if (!subTexture.attribute("name")) {
            impala::log::Debug(impala::log::LogCategory::Assets,
                               "Missing name attribute in SubTexture node in atlas file: {}",
                               _atlasPath);
            continue;
        }

        auto spriteNameNode = subTexture.attribute("name");
        auto xNode = subTexture.attribute("x");
        auto yNode = subTexture.attribute("y");
        auto widthNode = subTexture.attribute("width");
        auto heightNode = subTexture.attribute("height");

        if (!spriteNameNode || !xNode || !yNode || !widthNode || !heightNode) {
            impala::log::Debug(
                impala::log::LogCategory::Assets,
                "Missing one or more attributes in SubTexture node in atlas file: {}", _atlasPath);
            continue;
        }

        std::string spriteName = spriteNameNode.as_string();
        uint32_t x = xNode.as_uint();
        uint32_t y = yNode.as_uint();
        uint32_t width = widthNode.as_uint();
        uint32_t height = heightNode.as_uint();

        if (_spriteRects.find(spriteName) == _spriteRects.end()) {
            _spriteRects[spriteName] =
                SpriteRect{static_cast<uint32_t>(_spriteRects.size()), x, y, width, height};
            // TODO: Add trace logging for new sprite rects
            impala::log::Debug(impala::log::LogCategory::Assets,
                               "Added new sprite rect for '{}' in atlas file: {}", spriteName,
                               _atlasPath);
        } else {
            // handle duplicate sprite names if necessary
            // TODO: Add warning logging
        }
    }

    impala::log::Debug(impala::log::LogCategory::Assets,
                       "Finished initializing sprite atlas '{}' - '{}'", _name, sheetPath);
    _sheetPath = sheetPath;
    _initialized = true;
}

SpriteHandle SpriteAtlas::GetSprite(const std::string& name) {
    auto it = _spriteRects.find(name);
    if (it == _spriteRects.end()) {
        return InvalidSpriteHandle();
    }
    return SpriteHandle{it->second.id};
}

// ImpalaSprite SpriteAtlas::GetSprite(SpriteHandle spriteHandle) {
//     if (!spriteHandle.IsValid()) {
//         return ImpalaSprite{};
//     }

//     for (const auto& pair : _spriteRects) {
//         if (pair.second.id == spriteHandle.id) {
//             // Assuming you have a way to get the actual Sprite* from the SpriteRect
//             // This part depends on your implementation details
//             auto rect = pair.second;
//             return ImpalaSprite{rect, Texture{}};
//         }
//     }
//     return ImpalaSprite{};
// }