#include "dzone.h"
#include "impala/log.h"

impala::IGame* impala::CreateGame() {
    auto* game = new dzone::DzoneGame();
    log::Info(log::LogCategory::Game, "Creating Dzone game");
    return game;
}
