#pragma once
#include "dzone_assets.h"

namespace dzone {

    struct Projectile {
        const char* name;
        float speed = 0;
        float lifetime = 0;
        float thrust = 0;
        float homingTurnRate = 0;
        float damage = 0;
        float shieldPenetration = 0;
        float armorPenetration = 0;
        float cooldown = 0;
        const char* texturePath;
    };
    constexpr Projectile Laser() {
        return Projectile{.name = "Laser",
                          .speed = 100.0f,
                          .lifetime = 0.0f,
                          .thrust = 10.0f,
                          .homingTurnRate = .5f,
                          .damage = 1.0f,
                          .shieldPenetration = 1.0f,
                          .armorPenetration = 1.0f,
                          .cooldown = 0.5f,
                          .texturePath = "KENNEY_SPACE_SHOOTER_LASER_BLUE_01"};
    }

} // namespace dzone
