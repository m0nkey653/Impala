#include "impala_scene.h"

namespace impala {

    IScene* SceneFactory::CreateScene() {
        return new ImpalaScene();
    }

    void* ImpalaScene::AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                            std::unique_ptr<detail::IComponentHolder> component) {
        // Implement component addition logic here
        return nullptr;
    }

    void* ImpalaScene::GetComponentInternal(EntityHandle entity, uint32_t componentTypeId) {
        // Implement component retrieval logic here
        return nullptr;
    }

    EntityHandle ImpalaScene::CreateEntity(const char* name) {

        return EntityHandle{};
    }

    void ImpalaScene::DestroyEntity(EntityHandle entity) {
        // Implement entity destruction logic here
    }

} // namespace impala