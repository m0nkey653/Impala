#pragma once

#include "impala/scene.h"

namespace impala {

    struct Entity {
        uint32_t id;
        uint32_t generation;
        bool isActive;
    };

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

        EntityHandle CreateEntity(const char* name) override;
        void DestroyEntity(EntityHandle entity) override;

    protected:
        void* AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                   std::unique_ptr<detail::IComponentHolder> component) override;
        void* GetComponentInternal(EntityHandle entity, uint32_t componentTypeId) override;

    private:
        std::vector<Entity> _entities;
        std::unordered_map<uint32_t, std::unique_ptr<detail::IComponentHolder>> _components;
    };

} // namespace impala