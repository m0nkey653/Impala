#pragma once
#include "dzone_assets.h"
#include "impala/colliders.h"
#include "impala/math.h"

namespace dzone {

    struct ProjectileBluePrint {
        const char* name = nullptr;
        float speed = 0;
        float lifetime = 0;
        float thrust = 0;
        float homingTurnRate = 0;
        float damage = 0;
        float shieldPenetration = 0;
        float armorPenetration = 0;
        float cooldown = 0;
        impala::ColliderShape colliderShape = {};
        const char* spriteName = nullptr;
    };

    constexpr ProjectileBluePrint Laser() {
        return {
            .name = "Laser",
            .speed = 1000.0f,
            .lifetime = 5.0f,
            .thrust = 0.0f,
            .homingTurnRate = 0.0f,
            .damage = 10.0f,
            .shieldPenetration = 1.0f,
            .armorPenetration = 1.0f,
            .cooldown = 0.1f,
            .colliderShape = {.offset = {0, -19.2f},
                              .shape = impala::CircleCollider{.radius = 4.0f}},
            .spriteName = KENNEY_SPACE_SHOOTER_LASER_BLUE_01,
        };
    };

} // namespace dzone
