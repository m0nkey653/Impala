#pragma once
#include "impala/colliders.h"
#include "impala/transform.h"
#include "impala_scene.h"

namespace impala {
    // contains previous world transform
    struct PreviousTransform2d {
        Transform2d transform;
    };

    struct PreviousTransform3d {
        Transform3d transform;
    };

    class PhysicsSystem {
    public:
        PhysicsSystem() {};
        void ApplyVelocity(impala::ImpalaScene* scene, float deltaTime);

    private:
    };
} // namespace impala
