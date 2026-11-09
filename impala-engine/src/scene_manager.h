#pragma once

#include "impala/scene_manager.h"
#include "impala_scene.h"

namespace impala {

    class SceneManager : public ISceneManager {
    public:
        SceneManager() = default;
        ~SceneManager() override = default;

        IScene* CreateScene() override;

        IScene* GetActiveScene() override;

        ImpalaScene* GetActiveImpalaScene() { return _scenes[_activeSceneIndex].get(); };

    private:
        std::vector<std::unique_ptr<ImpalaScene>> _scenes;
        size_t _activeSceneIndex = 0;
    };

} // namespace impala