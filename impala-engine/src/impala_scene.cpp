#include "impala_scene.h"
#include "impala/transform.h"
#include "physics_system.h"
#include <cassert>

namespace impala {

    void ImpalaScene::OnComponentAddedInternal(EntityHandle entity, uint32_t typeId) {
        if (typeId == detail::ComponentTypeId<Transform2d>()) {
            AddComponent<WorldTransform2d>(entity);
            AddComponent<PreviousTransform2d>(entity);
        }
    }

    EntityHandle ImpalaScene::CreateEntity(const char* name) {
        for (size_t i = 0; i < _entities.size(); ++i) {
            auto e = &_entities[i];
            if (!e->isActive) {
                e->isActive = true;
                return EntityHandle{e->id, e->generation};
            }
        }
        Entity entity{
            .id = static_cast<uint32_t>(_entities.size()), .generation = 0, .isActive = true};
        _entities.push_back(entity);
        return EntityHandle{entity.id, entity.generation};
    }
    EntityHandle ImpalaScene::CreateEntity(const char* name, EntityHandle parent) {
        for (size_t i = 0; i < _entities.size(); ++i) {
            auto e = &_entities[i];
            if (!e->isActive) {
                e->isActive = true;
                return EntityHandle{e->id, e->generation};
            }
        }
        Entity entity{
            .id = static_cast<uint32_t>(_entities.size()), .generation = 0, .isActive = true};
        _entities.push_back(entity);
        return EntityHandle{entity.id, entity.generation};
    }

    EntityHandle ImpalaScene::CreateEntity(const char* name, math::Vec2 position, float rotation) {
        auto handle = CreateEntity(name);
        // Transform2d automatically adds Previous and World transform components
        auto& t = AddComponent<Transform2d>(handle);
        TeleportEntity(handle, position, rotation);
        return handle;
    }

    EntityHandle ImpalaScene::CreateEntity(const char* name, math::Vec2 position, float rotation,
                                           EntityHandle parent) {
        auto handle = CreateEntity(name);
        // Transform2d automatically adds Previous and World transform components
        auto& t = AddComponent<Transform2d>(handle);
        AddComponent<ParentEntity>(handle, ParentEntity{parent});
        TeleportEntity(handle, position, rotation);
        return handle;
    }

    void ImpalaScene::DestroyEntity(EntityHandle entity) {
        if (!IsAlive(entity)) {
            return; // normal "no entity"
        }
        _destroyedEntities.push_back(entity);
    }

    void ImpalaScene::FlushDestroyedEntities() {
        for (auto entity : _destroyedEntities) {
            auto e = &_entities[entity.id];
            if (e->generation != entity.generation || !e->isActive)
                continue;
            e->isActive = false;
            e->generation++;

            for (uint32_t i = 0; i < 64; ++i) {
                if (e->componentMask.test(i)) {
                    // uint32_t key = (entity.id << 16) | i;
                    _componentPools[i]->Remove(entity);
                }
            }
            e->componentMask.reset();
        }
        _destroyedEntities.clear();
    }

    void ImpalaScene::GetEntitiesWithMaskInternal(std::bitset<64> required,
                                                  std::vector<EntityHandle>& out) {
        for (auto& entity : _entities) {
            if (!entity.isActive)
                continue;
            if ((entity.componentMask & required) == required) {
                EntityHandle handle{entity.id, entity.generation};
                out.push_back(handle);
            }
        }
    }

    void ImpalaScene::TeleportEntity(EntityHandle entity, math::Vec2 position, float rotation) {
        if (!IsAlive(entity)) {
            return; // normal "no entity"
        }

        auto* transform = GetComponent<Transform2d>(entity);
        if (transform) {
            transform->position = position;
            impala::SetRotationDegrees(*transform, rotation);
            auto* previous = GetComponent<PreviousTransform2d>(entity);
            if (previous) {
                previous->transform = *transform;
            }
        }
    }

    void ImpalaScene::SetRotation(EntityHandle entity, float rotation) {
        if (!IsAlive(entity)) {
            return; // normal "no entity"
        }

        auto* transform = GetComponent<Transform2d>(entity);
        if (transform)
            transform->rotation = rotation;
    }

    const Transform2d* ImpalaScene::GetWorldTransform(EntityHandle entity) {
        if (!IsAlive(entity)) {
            return nullptr; // normal "no entity"
        }

        auto* world = GetComponent<WorldTransform2d>(entity);
        if (world) {
            return &world->transform;
        }
        return nullptr;
    }

    void ImpalaScene::ComputeWorldTransforms() {
        auto entities = GetEntitiesWithComponents<Transform2d, WorldTransform2d>();
        for (const auto& entity : entities) {
            auto t = GetComponent<impala::Transform2d>(entity);
            auto w = GetComponent<impala::WorldTransform2d>(entity);
            assert(t && w && "Entity with Transform2d is missing WorldTransform2d component");
            // TODO: once we have parent transforms, compute them here
            w->transform = *t;
        }
    }

    void ImpalaScene::SavePreviousTransforms(impala::ImpalaScene* scene) {
        auto entities = scene->GetEntitiesWithComponents<impala::WorldTransform2d,
                                                         impala::PreviousTransform2d>();
        for (const auto& entity : entities) {
            auto w = scene->GetComponent<impala::WorldTransform2d>(entity);
            auto prev = scene->GetComponent<impala::PreviousTransform2d>(entity);
            assert(w && prev &&
                   "Entity is missing WorldTransform2d or PreviousTransform2d "
                   "component");
            prev->transform = w->transform;
        }
    }

    bool ImpalaScene::IsAlive(EntityHandle entity) const {
        if (!entity.IsValid()) {
            return false; // normal "no entity"
        }
        assert(entity.id < _entities.size() && "EntityHandle id out of range");
        if (entity.id >= _entities.size()) {
            return false; // release: no crash
        }
        const Entity& e = _entities[entity.id];
        return e.isActive && e.generation == entity.generation; // stale handle → false
    }

    void ImpalaScene::SetComponentBitInternal(impala::EntityHandle entity, uint32_t typeId) {
        if (!IsAlive(entity)) {
            return; // normal "no entity"
        }
        if (typeId >= _componentPools.size()) {
            return; // no such pool
        }
        _entities[entity.id].componentMask.set(typeId);
    }

    impala::detail::IComponentPool* ImpalaScene::GetPoolInternal(uint32_t typeId) {
        if (typeId >= _componentPools.size()) {
            return nullptr;
        }
        return _componentPools[typeId].get();
    }

    impala::detail::IComponentPool* ImpalaScene::AddPoolInternal(
        uint32_t typeId, std::unique_ptr<impala::detail::IComponentPool,
                                         std::default_delete<impala::detail::IComponentPool>>
                             pool) {
        assert(typeId < 64 && "typeId out of range");
        if (typeId >= _componentPools.size()) {
            _componentPools.resize(typeId + 1);
        }
        _componentPools[typeId] = std::move(pool);
        return _componentPools[typeId].get();
    }

} // namespace impala
