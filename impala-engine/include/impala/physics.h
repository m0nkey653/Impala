#pragma once
#include "impala/math.h"

namespace impala {

    // current velocity and angular velocity of the ship
    struct Velocity2d {
        impala::math::Vec2 velocity;
        float angularVelocity = 0;
    };

} // namespace impala
