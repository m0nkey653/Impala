#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "assets/asset_database.h"
#include "impala/game.h"
#include "scene_manager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = NULL;
static SDL_Renderer* sdlRenderer = NULL;
static impala::IGame* impalaGame = NULL;
static std::unique_ptr<impala::AssetDatabase> assetDb;
static std::unique_ptr<impala::SceneManager> sceneManager;

// runs once at startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    SDL_SetHint(SDL_HINT_LOGGING, "*=debug");

    impalaGame = impala::CreateGame();

    SDL_SetAppMetadata(impalaGame->GetGameInfo().name.c_str(),
                       impalaGame->GetGameInfo().version.c_str(), nullptr);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    {
        SDL_Renderer* renderer = NULL;
        if (!SDL_CreateWindowAndRenderer(impalaGame->GetGameInfo().name.c_str(), 640, 480,
                                         SDL_WINDOW_RESIZABLE, &window, &renderer)) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
        sdlRenderer = renderer;
    }
    SDL_SetRenderLogicalPresentation(sdlRenderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    assetDb = std::make_unique<impala::AssetDatabase>(sdlRenderer);
    sceneManager = std::make_unique<impala::SceneManager>();
    impala::GameContext context;
    context.assetDataBase = assetDb.get();
    context.sceneManager = sceneManager.get();

    impalaGame->Init(&context);

    return SDL_APP_CONTINUE;
}

// callback when an event is received - input/mouse/keyboard/quit
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

// called once per frame (Update + Render)
SDL_AppResult SDL_AppIterate(void* appstate) {

    // locked physics update

    // game logic

    // render pass
    auto activeScene = sceneManager->GetActiveScene();

    return SDL_APP_CONTINUE;
}

// runs once at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    // SDL cleans up the window and renderer
}
