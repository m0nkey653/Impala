#pragma once

#include "impala/game.h"
#include "impala/scene.h"

namespace dzone {

    class MovementSystem {
    public:
        void Update(float deltaTime, impala::IScene* scene, impala::GameContext* gameContext);
    };

} // namespace dzone
