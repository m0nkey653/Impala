#pragma once

#include "impala/assets.h"
#include <string>

namespace impala {
    class ISprite : public IAsset {
    public:
        virtual ~ISprite() = default;
    };

    class ISpriteAtlas : public IAsset {
    public:
        virtual ~ISpriteAtlas() = default;
        virtual ISprite* GetSprite(const std::string& name) = 0;
    };

} // namespace impala
