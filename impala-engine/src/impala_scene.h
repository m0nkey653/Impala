#pragma once

#include "impala/scene.h"
#include <bitset>
#include <vector>

namespace impala {

    struct Entity {
        uint32_t id;
        uint32_t generation;
        bool isActive;
        std::bitset<64> componentMask;
    };

    class ImpalaScene : public IScene {
    public:
        ImpalaScene() = default;
        ~ImpalaScene() override = default;

        EntityHandle CreateEntity(const char* name) override;
        void DestroyEntity(EntityHandle entity) override;

        std::vector<Entity>& GetAllEntities() { return _entities; };
        void FlushDestroyedEntities();

    protected:
        void* AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                   std::unique_ptr<detail::IComponentHolder> component) override;
        void* GetComponentInternal(EntityHandle entity, uint32_t componentTypeId) override;
        void GetEntitiesWithMaskInternal(std::bitset<64> required,
                                         std::vector<EntityHandle>& out) override;

    private:
        std::vector<Entity> _entities;
        std::vector<EntityHandle> _destroyedEntities;
        std::unordered_map<uint32_t, std::unique_ptr<detail::IComponentHolder>> _components;
    };

} // namespace impala
