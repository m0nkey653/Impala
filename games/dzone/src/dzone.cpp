#include "dzone_assets.h"
#include "impala/game.h"
#include "impala/log.h"
#include "impala/sprite.h"
#include "impala/transform.h"
#include "version.h"

class DzoneGame : public impala::IGame {
public:
    impala::GameInfo GetGameInfo() const override { return {"Dzone", DZONE_VERSION}; }

    void Init(impala::GameContext* context) override {

        auto* assetDataBase = context->assetDataBase;
        assetDataBase->AddSpriteAtlas(dzone::KENNEY_SPACE_SHOOTER_SPRITE_ATLAS);
        auto player1sprite = assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_SHIP_1_BLUE);

        auto* scene = context->sceneManager->CreateScene();
        auto player = scene->CreateEntity("Player");
        scene->AddComponent<impala::Transform2d>(player);
        auto transform = scene->GetComponent<impala::Transform2d>(player);
        scene->AddComponent<impala::SpriteRenderer>(player);
        auto spriteRenderer = scene->GetComponent<impala::SpriteRenderer>(player);
        spriteRenderer->sprite = player1sprite;
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