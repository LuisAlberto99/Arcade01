#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "SceneManager.hpp"
#include "TitleScene.hpp"
#include "GameConstants.hpp"

struct AppState
{
    SDL_Renderer *renderer{nullptr};
    SDL_Window *window{nullptr};

    Uint64 last_ticks{0};
    float fixed_accumulator{0.0f};

    SceneManager sceneManager;
} appstate;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error al inicializar SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer("Pong - Proyecto 01", 
        static_cast<int>(GameConstants::WindowWidth), 
        static_cast<int>(GameConstants::WindowHeight), 
        0, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error al crear ventana o renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    ::appstate.window = window;
    ::appstate.renderer = renderer;
    ::appstate.last_ticks = SDL_GetTicks();

    // Cargamos la pantalla de título como estado inicial
    ::appstate.sceneManager.ChangeScene(std::make_unique<TitleScene>(&::appstate.sceneManager));
    ::appstate.sceneManager.ProcessPendingChanges();

    *appstate = &::appstate;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *app = static_cast<AppState *>(appstate);

    Uint64 current_ticks = SDL_GetTicks();
    float delta_time = static_cast<float>(current_ticks - app->last_ticks) / 1000.0f;
    app->last_ticks = current_ticks;

    // Evita la "espiral de la muerte" si el frame tardó demasiado (p.ej. se arrastró la ventana)
    if (delta_time > 0.25f)
    {
        delta_time = 0.25f;
    }

    app->sceneManager.ProcessPendingChanges();

    // Simulación física a paso fijo (60 Hz), desacoplada de la tasa de refresco visual
    app->fixed_accumulator += delta_time;
    while (app->fixed_accumulator >= GameConstants::FixedDeltaTime)
    {
        app->sceneManager.FixedUpdate(GameConstants::FixedDeltaTime);
        app->sceneManager.ProcessPendingChanges(); // aplica de inmediato una transición pedida en FixedUpdate
        app->fixed_accumulator -= GameConstants::FixedDeltaTime;
    }

    // Lógica general a la tasa de refresco variable (animaciones, parpadeos, etc.)
    app->sceneManager.Update(delta_time);

    app->sceneManager.Render(app->renderer);
    SDL_RenderPresent(app->renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *app = static_cast<AppState *>(appstate);

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    if (app && app->sceneManager.HasScenes())
    {
        app->sceneManager.HandleEvent(*event);
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    AppState *app = static_cast<AppState *>(appstate);
    if (app)
    {
        app->sceneManager.Clear();
        app->sceneManager.ProcessPendingChanges();
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
    }
    SDL_Quit();
}