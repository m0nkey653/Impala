#include "impala_scene.h"
#include "impala/transform.h"
#include "physics_system.h"
#include <cassert>

namespace impala {

    void* ImpalaScene::AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                            std::unique_ptr<detail::IComponentHolder> component) {
        auto e = &_entities[entity.id];
        if (e->generation != entity.generation || !e->isActive)
            return nullptr;

        uint32_t key = (entity.id << 16) | componentTypeId;
        _components[key] = std::move(component);
        e->componentMask.set(componentTypeId);

        if (componentTypeId == detail::ComponentTypeId<Transform2d>()) {
            AddComponentInternal(entity, detail::ComponentTypeId<WorldTransform2d>(),
                                 std::make_unique<detail::ComponentHolder<WorldTransform2d>>());
            AddComponentInternal(entity, detail::ComponentTypeId<PreviousTransform2d>(),
                                 std::make_unique<detail::ComponentHolder<PreviousTransform2d>>());
        }

        return _components[key]->Get();
    }

    void* ImpalaScene::GetComponentInternal(EntityHandle entity, uint32_t componentTypeId) {
        auto e = &_entities[entity.id];
        if (e->generation != entity.generation || !e->isActive)
            return nullptr;

        uint32_t key = (entity.id << 16) | componentTypeId;
        auto it = _components.find(key);
        if (it == _components.end())
            return nullptr;
        return it->second->Get();
    }

    EntityHandle ImpalaScene::CreateEntity(const char* name) {
        for (size_t i = 0; i < _entities.size(); ++i) {
            auto e = &_entities[i];
            if (!e->isActive) {
                e->isActive = true;
                return EntityHandle{e->id, e->generation};
            }
        }
        auto entity = new Entity();
        entity->id = static_cast<uint32_t>(_entities.size());
        entity->generation = 0;
        entity->isActive = true;
        _entities.push_back(*entity);
        return EntityHandle{entity->id, entity->generation};
    }

    EntityHandle ImpalaScene::CreateEntity(const char* name, math::Vec2 position, float rotation) {
        auto handle = CreateEntity(name);
        auto& t = AddComponent<Transform2d>(handle);
        TeleportEntity(handle, position, rotation);

        auto& prev = AddComponent<PreviousTransform2d>(handle);
        prev.transform = *GetComponent<Transform2d>(handle);
        prev.transform = t;
        auto& w = AddComponent<WorldTransform2d>(handle);
        w.transform = t;
        return handle;
    }

    void ImpalaScene::DestroyEntity(EntityHandle entity) {
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
                    uint32_t key = (entity.id << 16) | i;
                    _components.erase(key);
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
        auto e = &_entities[entity.id];
        if (e->generation != entity.generation || !e->isActive) {
            return;
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
        auto e = &_entities[entity.id];
        if (e->generation != entity.generation || !e->isActive)
            return;
        auto* transform = GetComponent<Transform2d>(entity);
        if (transform)
            transform->rotation = rotation;
    }

    const Transform2d* ImpalaScene::GetWorldTransform(EntityHandle entity) {
        auto e = &_entities[entity.id];
        if (e->generation != entity.generation || !e->isActive) {
            return nullptr;
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

} // namespace impala
