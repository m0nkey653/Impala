#include "impala/game.h"
#include "version.h"

class GolfGame : public impala::IGame {
public:
    impala::GameInfo GetGameInfo() const override { return {"Golf", GOLF_VERSION}; }

    void Init(impala::GameContext* context) override {
        // Initialization code here
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
    return new GolfGame();
}