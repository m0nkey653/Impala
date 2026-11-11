#pragma once

#include "impala/handles.h"
#include "impala/math.h"
#include "projectile.h"
#include <vector>

namespace dzone {
    struct ShipControls {
        float throttle = 0;
        float turn = 0;
        bool firePrimary = false;
        bool fireSecondary = false;
    };

    struct Thrust {
        float acceleration = 0;
        float maxSpeed = 0;
        // how fast the ship slows down due to drag
        float drag = 0;
        // how fast the ship can change direction of movement
        float grip = 0;
        float turnRate = 0;
    };

    struct Movement {
        impala::math::Vec2 velocity;
        float angularVelocity = 0;
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

    struct Target {
        impala::EntityHandle target;
    };

    struct Lifetime {
        float remaining = 0;
    };

    struct WeaponSlot {
        ProjectileBluePrint projectile;
        impala::math::Vec2 muzzleOffset; // where it fires from, relative to the ship
        float cooldownRemaining = 0;
    };

    struct Weapons {
        std::vector<WeaponSlot> slots; // or std::array<WeaponSlot, 4> plus a count
        int activePrimary = 0;         // index into slots
        int activeSecondary = -1;      // -1 = none equipped
    };

} // namespace dzone
