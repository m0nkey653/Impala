#pragma once
#include "components.h"
#include "math.h"

namespace impala {

    class Transform2d : public IComponent {
    public:
        math::Vec2 position;
        float rotation;
        math::Vec2 scale;
    };

    class Transform3d : public IComponent {
    public:
        math::Vec3 position;
        math::Vec3 rotation;
        math::Vec3 scale;
    };

} // namespace impala