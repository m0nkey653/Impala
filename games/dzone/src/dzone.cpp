#include "impala/game.h"
#include "version.h"

class DzoneGame : public impala::IGame {
public:
    impala::GameInfo GetGameInfo() const override { return {"Dzone", DZONE_VERSION}; }

    void Init(impala::GameContext* context) override {
        // Initialization code here
        auto* assetDataBase = context->assetDataBase;
        assetDataBase->AddSpriteAtlas("", "");

        auto* scene = context->sceneFactory->CreateScene();
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
    printf("Creating Dzone game\n");
    return game;
}