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
        auto playerSprite0 = assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_SHIP_1_BLUE);
        auto playerSprite1 = assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_SHIP_2_GREEN);
        assetDataBase->LoadSpriteTexture(playerSprite0);
        assetDataBase->LoadSpriteTexture(playerSprite1);

        auto* scene = context->sceneManager->CreateScene();

        auto player0 = scene->CreateEntity("Player 1");
        scene->AddComponent<impala::Transform2d>(player0);
        auto transform = scene->GetComponent<impala::Transform2d>(player0);
        transform->position = {100.0f, 100.0f}; // Set initial position for the player
        transform->rotation = 90;
        scene->AddComponent<impala::SpriteRenderer>(player0);
        auto spriteRenderer = scene->GetComponent<impala::SpriteRenderer>(player0);
        spriteRenderer->sprite = playerSprite0;

        auto player1 = scene->CreateEntity("Player 2");
        scene->AddComponent<impala::Transform2d>(player1);
        auto transform1 = scene->GetComponent<impala::Transform2d>(player1);
        transform1->position = {400.0f, 100.0f}; // Set initial position for the player
        transform1->rotation = -90;
        scene->AddComponent<impala::SpriteRenderer>(player1);
        auto spriteRenderer1 = scene->GetComponent<impala::SpriteRenderer>(player1);
        spriteRenderer1->sprite = playerSprite1;
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