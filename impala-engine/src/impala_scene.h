#pragma once

#include "impala/scene.h"

namespace impala {

    class SceneFactory : public ISceneFactory {
    public:
        SceneFactory() = default;
        ~SceneFactory() override = default;

        IScene* CreateScene() override;
    };

    class ImpalaScene : public IScene {
    public:
        ImpalaScene() = default;
        ~ImpalaScene() override = default;

    protected:
        void* AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                   std::unique_ptr<detail::IComponentHolder> component) override;
        void* GetComponentInternal(EntityHandle entity, uint32_t componentTypeId) override;
    };

} // namespace impala