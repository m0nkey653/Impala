#pragma once

#include "impala/math.h"
#include "impala/scene.h"
#include <bitset>
#include <cassert>
#include <vector>

namespace impala {

    struct Entity {
        uint32_t id = 0;
        uint32_t generation = 0;
        bool isActive = false;
        bool isBeingDestroyed = false;
        std::bitset<64> componentMask = {};
    };

    struct Parent {
        EntityHandle firstChild = EntityHandle::Invalid();
        EntityHandle lastChild = EntityHandle::Invalid();
        uint32_t childCount = 0;
    };

    struct Child {
        EntityHandle parent = EntityHandle::Invalid();
        EntityHandle nextSibling = EntityHandle::Invalid();
        EntityHandle prevSibling = EntityHandle::Invalid();
    };

    class ImpalaScene : public IScene {
    public:
        ImpalaScene() = default;
        ~ImpalaScene() override = default;

        EntityHandle CreateEntity(const char* name) override;
        EntityHandle CreateEntity(const char* name, EntityHandle parent) override;
        EntityHandle CreateEntity(const char* name, math::Vec2 position, float rotation) override;
        EntityHandle CreateEntity(const char* name, math::Vec2 position, float rotation,
                                  EntityHandle parent) override;
        void DestroyEntity(EntityHandle entity) override;
        const Transform2d* GetWorldTransform(EntityHandle entity) override;

        impala::detail::IComponentPool* GetPoolInternal(uint32_t) override;
        impala::detail::IComponentPool* AddPoolInternal(
            uint32_t,
            std::unique_ptr<impala::detail::IComponentPool,
                            std::default_delete<impala::detail::IComponentPool>>) override;
        void SetComponentBitInternal(impala::EntityHandle, uint32_t) override;

        std::vector<Entity>& GetAllEntities() { return _entities; };
        void FlushDestroyedEntities();
        void ComputeWorldTransforms();
        void SavePreviousTransforms(impala::ImpalaScene* scene);
        bool IsAlive(EntityHandle entity) const override;
        void LinkEntity(EntityHandle parent, EntityHandle child) override;
        void UnlinkEntity(EntityHandle child) override;

    protected:
        void GetEntitiesWithMaskInternal(std::bitset<64> required,
                                         std::vector<EntityHandle>& out) override;
        void TeleportEntity(EntityHandle entity, math::Vec2 position, float rotation) override;
        void SetRotation(EntityHandle entity, float rotation) override;
        void OnComponentAddedInternal(EntityHandle entity, uint32_t typeId) override;

    private:
        bool _entitiesDirty = false;
        std::vector<Entity> _entities;
        std::vector<EntityHandle> _destroyedEntities;
        std::vector<std::unique_ptr<detail::IComponentPool>> _componentPools;
    };

} // namespace impala
