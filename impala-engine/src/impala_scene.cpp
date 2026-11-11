#include "impala_scene.h"

namespace impala {

    void* ImpalaScene::AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                            std::unique_ptr<detail::IComponentHolder> component) {
        auto e = &_entities[entity.id];
        if (e->generation != entity.generation || !e->isActive)
            return nullptr;

        uint32_t key = (entity.id << 16) | componentTypeId;
        _components[key] = std::move(component);
        e->componentMask.set(componentTypeId);
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

} // namespace impala
