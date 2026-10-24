#include "impala/scenes.h"

namespace impala {

    class Scene : public IScene {
    public:
        Scene() = default;
        ~Scene() override = default;

    protected:
        void* AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                   std::unique_ptr<detail::IComponentHolder> component) override;
        void* GetComponentInternal(EntityHandle entity, uint32_t componentTypeId) override;
    };

    class SceneFactory : public ISceneFactory {
    public:
        SceneFactory() = default;
        ~SceneFactory() override = default;

        IScene* CreateScene() override { return new Scene(); }
    };

} // namespace impala