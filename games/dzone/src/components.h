#pragma once

#include "impala/handles.h"
#include "impala/math.h"
#include "projectile.h"

namespace dzone {
    struct Movement {
        impala::math::Vec2 velocity;
        float angularVelocity = 0;
    };

    struct Thrust {
        float maxSpeed = 0;
        float acceleration = 0;
        float throttle = 0;
    };

    struct Health {
        float current = 0;
        float maximum = 0;
    };

    struct Shield {
        float current = 0;
        float maximum = 0;
    };

    struct Armor {
        float current = 0;
        float maximum = 0;
    };

    struct Damage {
        float amount = 0;
        float shieldPenetration = 0;
        float armorPenetration = 0;
    };

    struct Homing {
        float turnRate = 0;
    };

    struct Target {
        impala::EntityHandle target;
    };

    struct Lifetime {
        float remaining = 0;
    };

    struct WeaponSlot {
        Projectile projectile;
        impala::math::Vec2 muzzleOffset; // where it fires from, relative to the ship
        float cooldownRemaining = 0;
        bool triggerHeld = false;
    };

    struct Weapons {
        std::vector<WeaponSlot> slots; // or std::array<WeaponSlot, 4> plus a count
        bool primaryHeld = false;
        bool secondaryHeld = false;
        int activePrimary = 0;    // index into slots
        int activeSecondary = -1; // -1 = none equipped
    };

} // namespace dzone
