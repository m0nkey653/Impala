#include "impala/game.h"

class DzoneGame : public impala::Game {
public:
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

impala::Game* CreateGame() {
    return new DzoneGame();
}