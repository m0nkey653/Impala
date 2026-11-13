#pragma once

#include "dzone_assets.h"
#include "impala/game.h"
#include "impala/scene.h"
#include "player_collection.h"
#include "version.h"

namespace dzone {
    class DzoneGame : public impala::IGame {
    public:
        impala::GameInfo GetGameInfo() const override {
            return {"Dzone", DZONE_VERSION, dzone::DZONE_ICON};
        }
        void Init(impala::GameContext* context) override;
        void Update(impala::GameContext* context, float dt) override;
        void Render(impala::GameContext* context) override;
        void OnEvent(const impala::GameEvent& event) override;
        void OnGamepadConnected(impala::GamepadHandle handle) override;
        void OnGamepadDisconnected(impala::GamepadHandle handle) override;
        void Shutdown() override;

    private:
        impala::IScene* _activeScene = nullptr;
        PlayerCollection _players;
    };
} // namespace dzone
