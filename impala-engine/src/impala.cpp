#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "assets/asset_database.h"
#include "impala/game.h"
#include "impala/handles.h"
#include "impala/transform.h"
#include "input/gamepad_collection.h"
#include "input/keyboard.h"
#include "physics_system.h"
#include "renderer.h"
#include "scene_manager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cassert>

static SDL_Window* window = nullptr;
static std::unique_ptr<impala::Renderer> sdlRenderer;
static std::unique_ptr<impala::IGame> impalaGame;
static std::unique_ptr<impala::AssetDatabase> assetDb;
static std::unique_ptr<impala::SceneManager> sceneManager;
static std::unique_ptr<impala::GamepadCollection> gamepadCollection;
static std::unique_ptr<impala::Keyboard> keyboard;
static std::unique_ptr<impala::PhysicsSystem> physicsSystem;

impala::GameContext context{};
uint64_t lastTicks = 0;
bool firstFrame = true;
float gameSpeed = 1.0f;

float fixedAccumulator = 0.0f;
const float FIXED_TIMESTEP = 1.0f / 60.0f;

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
    physicsSystem = std::make_unique<impala::PhysicsSystem>();

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
    firstFrame = false;
    deltaTime *= std::clamp(gameSpeed, 0.0f, 1.0f);
    lastTicks = currentTicks;

    auto activeScene = sceneManager->GetActiveImpalaScene();

    // get input states
    gamepadCollection->Update(deltaTime);
    // locked physics update
    // add a flush for destroyed during the fixed update

    fixedAccumulator = std::min(fixedAccumulator += deltaTime, FIXED_TIMESTEP * 5);
    while (fixedAccumulator >= FIXED_TIMESTEP) {
        physicsSystem->SavePreviousTransforms(activeScene);
        impalaGame->FixedUpdate(&context, FIXED_TIMESTEP);

        // physics simulation -> move/rotate transforms with Velocity
        physicsSystem->ApplyVelocity(activeScene, FIXED_TIMESTEP);
        // calculate collisions - accumulate collision responses
        fixedAccumulator -= FIXED_TIMESTEP;

        // flush anything destroyed in this fixed update
        activeScene->FlushDestroyedEntities();

        // flush fixed gamepad/keyboard states
        gamepadCollection->PostFixedUpdate();
        keyboard->PostFixedUpdate();
    }

    float alpha = fixedAccumulator / FIXED_TIMESTEP;

    // game logic outside of physics
    impalaGame->Update(&context, deltaTime);

    // flush anything destroyed in this frame
    activeScene->FlushDestroyedEntities();

    // flush frame gamepad/keyboard states
    gamepadCollection->PostUpdate();
    keyboard->PostUpdate();

    // render pass
    sdlRenderer->RenderFrame(activeScene, assetDb.get(), alpha);

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
