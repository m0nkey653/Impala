#include "impala_scene.h"

namespace impala {

    IScene* SceneFactory::CreateScene() {
        return new ImpalaScene();
    }

    void* ImpalaScene::AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                            std::unique_ptr<detail::IComponentHolder> component) {
        auto e = &_entities[entity.id];
        if (e->generation != entity.generation || !e->isActive)
            return nullptr;

        uint32_t key = (entity.id << 16) | componentTypeId;
        _components[key] = std::move(component);
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
        auto entity = new Entity();
        entity->id = static_cast<uint32_t>(_entities.size());
        entity->generation = 0;
        entity->isActive = true;
        _entities.push_back(*entity);
        return EntityHandle{entity->id, entity->generation};
    }

    void ImpalaScene::DestroyEntity(EntityHandle entity) {
        // Implement entity destruction logic here
    }

} // namespace impala