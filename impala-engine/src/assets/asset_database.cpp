#include "asset_database.h"

#include <pugixml.hpp>

void AssetDatabase::AddSpriteAtlas(const std::string& name, const std::string& atlasPath) {
    // load SDL image
    // parse atlas file
    // populate dictionary with name <x, y, width, height> mappings

    pugi::xml_document doc;
    doc.load_file(atlasPath.c_str());

    pugi::xml_node textureAtlas = doc.child("TextureAtlas");
    for (pugi::xml_node subTexture : textureAtlas.children("SubTexture")) {
        std::string spriteName = subTexture.attribute("name").as_string();
        int x = subTexture.attribute("x").as_int();
        int y = subTexture.attribute("y").as_int();
        int width = subTexture.attribute("width").as_int();
        int height = subTexture.attribute("height").as_int();
        // populate dictionary with name <x, y, width, height> mappings
        _spriteRects[spriteName] = std::make_tuple(x, y, width, height);
    }
}

/*
Sparrow/Starling atlas format
This format is used by the Sparrow and Starling frameworks for defining sprite atlases.
Example:
<TextureAtlas imagePath="sheet.png">
        <SubTexture name="beam0.png" x="143" y="377" width="43" height="31"/>
        <SubTexture name="beam1.png" x="327" y="644" width="40" height="20"/>
</TextureAtlas>
*/
