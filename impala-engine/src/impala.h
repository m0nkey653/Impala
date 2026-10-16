#pragma once

enum class LogCategory : int {
    Engine = SDL_LOG_CATEGORY_CUSTOM,
    Assets,
    Scene,
    Game,
};