#pragma once

#include "impala/game.h"

namespace dzone {
    class WeaponsSystem {
    public:
        void Update(float deltaTime, impala::IScene* scene, impala::GameContext* gameContext);
    };
} // namespace dzone
