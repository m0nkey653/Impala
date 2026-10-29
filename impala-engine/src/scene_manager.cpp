#include "scene_manager.h"
#include "impala_scene.h"

namespace impala {

    IScene* SceneManager::CreateScene() {
        auto scene = std::make_unique<ImpalaScene>();
        _scenes.push_back(std::move(scene));
        return _scenes.back().get();
    }

    IScene* SceneManager::GetActiveScene() {
        if (_scenes.empty()) {
            throw ImpalaError("No active scene available");
        }
        return _scenes[_activeSceneIndex].get();
    }
} // namespace impala