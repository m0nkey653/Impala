#include "impala/game.h"
#include "version.h"

class DzoneGame : public impala::Game {
public:
    impala::GameInfo GetGameInfo() const override { return {"Dzone", DZONE_VERSION}; }

    void Init() override {
        // Initialization code here
    }

    void Update(float dt) override {
        // Update game state here
    }

    void Render(impala::IGameRender* render) override {
        // Render game here
    }

    void OnEvent(const impala::GameEvent& event) override {
        // Handle game events here
    }

    void Shutdown() override {
        // Cleanup code here
    }
};

impala::Game* impala::CreateGame() {
    return new DzoneGame();
}