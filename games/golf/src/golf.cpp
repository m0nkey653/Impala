#include "impala/game.h"
#include "version.h"

class GolfGame : public impala::Game {
public:
    impala::GameInfo GetGameInfo() const override { return {"Golf", GOLF_VERSION}; }

    void Init() override {
        // Initialization code here
    }

    void Update(float dt) override {
        // Update game state here
    }

    void Render(impala::IGameRender* render) override {
        // Render game here
    }

    void OnEvent(const impala::ImpalaGameEvent& event) override {
        // Handle game events here
    }

    void Shutdown() override {
        // Cleanup code here
    }

    void run() override {
        // Main game loop here
    }
};

impala::Game* impala::CreateGame() {
    return new GolfGame();
}