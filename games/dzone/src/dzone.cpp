#include "dzone_assets.h"
#include "impala/game.h"
#include "impala/log.h"
#include "version.h"

class DzoneGame : public impala::IGame {
public:
    impala::GameInfo GetGameInfo() const override { return {"Dzone", DZONE_VERSION}; }

    void Init(impala::GameContext* context) override {

        auto* assetDataBase = context->assetDataBase;
        assetDataBase->AddSpriteAtlas("kenney-space", dzone::KENNEY_SPACE_SHOOTER);

        impala::log::Debug(impala::log::LogCategory::Assets, "Added sprite atlas 'kenney-space'");
        auto* scene = context->sceneFactory->CreateScene();
        impala::log::Debug(impala::log::LogCategory::Assets, "Created new scene");
    }

    void Update(impala::GameContext* context, float dt) override {
        // Update game state here
    }

    void Render(impala::GameContext* context) override {
        // Render game here
    }

    void OnEvent(const impala::GameEvent& event) override {
        // Handle game events here
    }

    void Shutdown() override {
        // Cleanup code here
    }
};

impala::IGame* impala::CreateGame() {
    auto* game = new DzoneGame();
    log::Info(log::LogCategory::Game, "Creating Dzone game");
    return game;
}