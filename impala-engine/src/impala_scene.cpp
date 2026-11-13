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

    // std::vector<std::tuple<void*, void*, void*, void*>>
    // ImpalaScene::GetEntitiesWithComponentsInternal(uint32_t componentTypeIdT,
    //                                                uint32_t componentTypeIdU,
    //                                                uint32_t componentTypeIdV,
    //                                                uint32_t componentTypeIdW) {

    //     std::vector<std::tuple<void*, void*, void*, void*>> result;
    //     void* componentPtrT = nullptr;
    //     void* componentPtrU = nullptr;
    //     void* componentPtrV = nullptr;
    //     void* componentPtrW = nullptr;

    //     for (auto& entity : _entities) {
    //         impala::log::Debug(impala::log::LogCategory::Scene, "Processing entity {}",
    //         entity.id); if (!entity.isActive)
    //             continue;
    //         if (entity.componentMask.test(componentTypeIdT))
    //             componentPtrT = _components[(entity.id << 16) | componentTypeIdT]->Get();
    //         if (entity.componentMask.test(componentTypeIdU))
    //             componentPtrU = _components[(entity.id << 16) | componentTypeIdU]->Get();
    //         if (entity.componentMask.test(componentTypeIdV))
    //             componentPtrV = _components[(entity.id << 16) | componentTypeIdV]->Get();
    //         if (entity.componentMask.test(componentTypeIdW))
    //             componentPtrW = _components[(entity.id << 16) | componentTypeIdW]->Get();
    //         result.push_back(
    //             std::make_tuple(componentPtrT, componentPtrU, componentPtrV, componentPtrW));

    //         impala::log::Debug(impala::log::LogCategory::Scene,
    //                            "Added entity {} to result {:p} {:p} {:p} {:p}", entity.id,
    //                            componentPtrT, componentPtrU, componentPtrV, componentPtrW);
    //     }

    //     return result;
    // }

    void ImpalaScene::GetEntitiesWithMaskInternal(std::bitset<64> required,
                                                  std::vector<EntityHandle>& out) {
        std::vector<EntityHandle> result;
        for (auto& entity : _entities) {
            if (!entity.isActive)
                continue;
            if ((entity.componentMask & required) == required) {
                EntityHandle handle{entity.id, entity.generation};
                out.push_back(handle);
            }
        }
    }

} // namespace impala
