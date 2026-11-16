#pragma once
#include "impala/transform.h"
#include "impala_scene.h"

namespace impala {
    struct PreviousTransform2d {
        Transform2d transform;
    };

    struct PreviousTransform3d {
        Transform3d transform;
    };

    class PhysicsSystem {
    public:
        void SavePreviousTransforms(impala::ImpalaScene* scene);
        void ApplyVelocity(impala::ImpalaScene* scene, float deltaTime);
    };
} // namespace impala
