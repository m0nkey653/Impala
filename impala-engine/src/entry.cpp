#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "assets/asset_database.h"
#include "impala/game.h"
#include "impala/handles.h"
#include "impala/transform.h"
#include "renderer.h"
#include "scene_manager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = nullptr;
static std::unique_ptr<impala::Renderer> sdlRenderer;
static impala::IGame* impalaGame = nullptr;
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
        SDL_Renderer* renderer = nullptr;
        if (!SDL_CreateWindowAndRenderer(impalaGame->GetGameInfo().name.c_str(), 640, 480,
                                         SDL_WINDOW_RESIZABLE, &window, &renderer)) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
        sdlRenderer = std::make_unique<impala::Renderer>(renderer);
    }
    SDL_SetRenderLogicalPresentation(sdlRenderer->Get(), 640, 480,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    assetDb = std::make_unique<impala::AssetDatabase>(sdlRenderer.get());
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

bool firstFrame = true;
// called once per frame (Update + Render)
SDL_AppResult SDL_AppIterate(void* appstate) {

    // locked physics update

    // game logic

    // render pass
    // SDL_ClearRender(sdlRenderer->Get());
    SDL_SetRenderDrawColor(sdlRenderer->Get(), 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer->Get());
    SDL_SetRenderDrawColor(sdlRenderer->Get(), 255, 255, 255, SDL_ALPHA_OPAQUE);

    auto activeScene = sceneManager->GetActiveImpalaScene();
    for (const auto& entity : activeScene->GetAllEntities()) {
        auto r = activeScene->GetComponent<impala::SpriteRenderer>(
            impala::EntityHandle{entity.id, entity.generation});
        if (!r) {
            continue;
        }
        auto t = activeScene->GetComponent<impala::Transform2d>(
            impala::EntityHandle{entity.id, entity.generation});
        if (!t) {
            continue;
        }
        auto sprite = assetDb->GetSprite(r->sprite);
        auto texture = assetDb->GetTexture(sprite.textureHandle);

        SDL_FRect srcRect = sprite.rect.ToSDLFRect();
        SDL_FRect dstRect = {t->position.x, t->position.y, srcRect.w, srcRect.h};

        bool result = false;
        if (t->rotation != 0) {
            result = SDL_RenderTextureRotated(sdlRenderer->Get(), texture->GetTexture(), &srcRect,
                                              &dstRect, t->rotation, nullptr, SDL_FLIP_NONE);
        } else {
            result =
                SDL_RenderTexture(sdlRenderer->Get(), texture->GetTexture(), &srcRect, &dstRect);
        }
        if (!result) {
            impala::log::Error(impala::log::LogCategory::Engine, "Failed to render texture: {}",
                               SDL_GetError());
        }
    }
    SDL_RenderPresent(sdlRenderer->Get());

    return SDL_APP_CONTINUE;
}

// runs once at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    // SDL cleans up the window and renderer
}
