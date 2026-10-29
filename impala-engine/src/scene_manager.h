#pragma once

#include "impala/scene_manager.h"

namespace impala {

    class SceneManager : public ISceneManager {
    public:
        SceneManager() = default;
        ~SceneManager() override = default;

        IScene* CreateScene() override;

        IScene* GetActiveScene() override;

    private:
        std::vector<std::unique_ptr<IScene>> _scenes;
        size_t _activeSceneIndex = 0;
    };

} // namespace impala