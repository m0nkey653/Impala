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
        PhysicsSystem() { _collisions.reserve(64); };
        void ApplyVelocity(impala::ImpalaScene* scene, float deltaTime);
        std::span<Collision> CalculateCollisions(impala::ImpalaScene* scene);

    private:
        std::vector<Collision> _collisions;
    };
} // namespace impala
