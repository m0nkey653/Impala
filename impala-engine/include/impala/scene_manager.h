#pragma once

#include "scene.h"

namespace impala {
    class ISceneManager {
    public:
        virtual ~ISceneManager() = default;
        virtual IScene* CreateScene() = 0;
        virtual IScene* GetActiveScene() = 0;
    };
} // namespace impala