#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "assets/asset_database.h"
#include "impala/game.h"
#include "impala/handles.h"
#include "impala/transform.h"
#include "input/gamepad_collection.h"
#include "input/keyboard.h"
#include "renderer.h"
#include "scene_manager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = nullptr;
static std::unique_ptr<impala::Renderer> sdlRenderer;
static std::unique_ptr<impala::IGame> impalaGame;
static std::unique_ptr<impala::AssetDatabase> assetDb;
static std::unique_ptr<impala::SceneManager> sceneManager;
static std::unique_ptr<impala::GamepadCollection> gamepadCollection;
static std::unique_ptr<impala::Keyboard> keyboard;

impala::GameContext context{};
uint64_t lastTicks = 0;
bool firstFrame = true;
float gameSpeed = 1.0f;

struct AppState {};

// runs once at startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    SDL_SetHint(SDL_HINT_LOGGING, "*=debug");

    impalaGame = std::unique_ptr<impala::IGame>(impala::CreateGame());

    SDL_SetAppMetadata(impalaGame->GetGameInfo().name, impalaGame->GetGameInfo().version, nullptr);

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    {
        SDL_Renderer* renderer = nullptr;
        if (!SDL_CreateWindowAndRenderer(impalaGame->GetGameInfo().name, 1920, 1080,
                                         SDL_WINDOW_RESIZABLE, &window, &renderer)) {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
        auto surface = SDL_LoadSurface(impalaGame->GetGameInfo().iconPath);
        if (surface) {
            SDL_SetWindowIcon(window, surface);
            SDL_DestroySurface(surface);
        } else {
            impala::log::Warn(impala::log::LogCategory::Engine, "Failed to load window icon: {}",
                              SDL_GetError());
        }
        sdlRenderer = std::make_unique<impala::Renderer>(renderer);
    }

    SDL_SetRenderLogicalPresentation(sdlRenderer->Get(), 1920, 1080,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    assetDb = std::make_unique<impala::AssetDatabase>(sdlRenderer.get());
    sceneManager = std::make_unique<impala::SceneManager>();
    gamepadCollection = std::make_unique<impala::GamepadCollection>();
    keyboard = std::make_unique<impala::Keyboard>();

    context.assetDataBase = assetDb.get();
    context.sceneManager = sceneManager.get();
    context.gamepads = gamepadCollection.get();
    context.keyboard = keyboard.get();

    impala::log::Debug(impala::log::LogCategory::Engine, "Calling init on game: {}",
                       impalaGame->GetGameInfo().name);
    impalaGame->Init(&context);

    return SDL_APP_CONTINUE;
}

// callback when an event is received - input/mouse/keyboard/quit
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_GAMEPAD_ADDED: {
        auto handle = gamepadCollection->GamepadConnected(event->gdevice.which);
        if (handle.IsValid()) {
            impalaGame->OnGamepadConnected(handle);
        }
        break;
    }
    case SDL_EVENT_KEY_UP:
        switch (event->key.scancode) {
        case SDL_SCANCODE_PAUSE:
            gameSpeed = (gameSpeed == 1.0f) ? 0.0f : 1.0f;
            break;
        case SDL_SCANCODE_PAGEUP:
            gameSpeed = std::min(gameSpeed + 0.1f, 1.0f);
            break;
        case SDL_SCANCODE_PAGEDOWN:
            gameSpeed = std::max(gameSpeed - 0.1f, 0.0f);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

// called once per frame (Update + Render)
SDL_AppResult SDL_AppIterate(void* appstate) {
    auto currentTicks = SDL_GetTicksNS();
    auto deltaTime = firstFrame ? 0.0f : (currentTicks - lastTicks) / 1e9f;
    deltaTime *= std::clamp(gameSpeed, 0.0f, 1.0f);
    lastTicks = currentTicks;
    firstFrame = false;

    // get input states
    gamepadCollection->Update(deltaTime); // Assuming dt is 0 for now

    auto activeScene = sceneManager->GetActiveImpalaScene();

    // locked physics update
    // add a flush for destroyed during the fixed update
    // activeScene->FlushDestroyedEntities();

    // game logic
    impalaGame->Update(&context, deltaTime);

    // flush anything destroyed in this frame
    activeScene->FlushDestroyedEntities();

    // post-update input states
    gamepadCollection->PostUpdate();
    keyboard->PostUpdate();

    // render pass
    // SDL_ClearRender(sdlRenderer->Get());
    SDL_SetRenderDrawColor(sdlRenderer->Get(), 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer->Get());
    SDL_SetRenderDrawColor(sdlRenderer->Get(), 255, 255, 255, SDL_ALPHA_OPAQUE);

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
        auto anchor = r->anchor.value_or(sprite.anchor);
        SDL_FPoint pivot = {(1 + anchor.x) * sprite.rect.width * 0.5f,
                            (1 + anchor.y) * sprite.rect.height * 0.5f};
        impala::math::Vec2 topLeft = t->position - impala::math::Vec2{pivot.x, pivot.y};
        SDL_FRect dstRect = {topLeft.x, topLeft.y, srcRect.w, srcRect.h};

        bool result = false;
        if (t->rotation != 0) {
            result = SDL_RenderTextureRotated(sdlRenderer->Get(), texture->GetTexture(), &srcRect,
                                              &dstRect, t->rotation, &pivot, SDL_FLIP_NONE);
        } else {
            result =
                SDL_RenderTexture(sdlRenderer->Get(), texture->GetTexture(), &srcRect, &dstRect);
        }
        if (!result) {
            impala::log::Error(impala::log::LogCategory::Engine, "Failed to render texture: {}",
                               SDL_GetError());
        }

        float px = std::round(t->position.x);
        float py = std::round(t->position.y);
        SDL_FRect debugTransform = {px - 2.0f, py - 2.0f, 4.0f, 4.0f};
        SDL_FRect debugTransformOutline = {px - 4.0f, py - 4.0f, 8.0f, 8.0f};
        SDL_SetRenderDrawColor(sdlRenderer->Get(), 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(sdlRenderer->Get(), &debugTransform);
        SDL_SetRenderDrawColor(sdlRenderer->Get(), 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderRect(sdlRenderer->Get(), &debugTransformOutline);
    }

    SDL_RenderPresent(sdlRenderer->Get());

    return SDL_APP_CONTINUE;
}

// runs once at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    // SDL cleans up the window - we manually release the renderer and other resources
    impala::log::Info(impala::log::LogCategory::Engine, "Shutting down application");
    sceneManager.reset();
    assetDb.reset();
    impalaGame.reset();
    sdlRenderer.reset();
}
