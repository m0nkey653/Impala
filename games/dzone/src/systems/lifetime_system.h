#pragma once

namespace impala {
    class IScene;
}

namespace dzone {
    class LifetimeSystem {
    public:
        void Update(impala::IScene* scene, float deltaTime);
    };

} // namespace dzone