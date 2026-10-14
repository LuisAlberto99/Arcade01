#include "PauseScene.hpp"
#include "SceneManager.hpp"
#include "BitmapFont.hpp"
#include "GameConstants.hpp"

void PauseScene::Render(SDL_Renderer *renderer)
{
    // Superposición semitransparente sobre la partida congelada
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
    SDL_FRect overlay{0.0f, 0.0f, GameConstants::WindowWidth, GameConstants::WindowHeight};
    SDL_RenderFillRect(renderer, &overlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_Color white{255, 255, 255, 255};
    const float titlePixel = 8.0f;
    float titleWidth = BitmapFont::MeasureTextWidth("PAUSA", titlePixel);
    BitmapFont::DrawText(renderer, "PAUSA", (GameConstants::WindowWidth - titleWidth) * 0.5f, 220.0f, titlePixel, white);

    const float instrPixel = 4.0f;
    std::string line = "P: CONTINUAR";
    float lineWidth = BitmapFont::MeasureTextWidth(line, instrPixel);
    BitmapFont::DrawText(renderer, line, (GameConstants::WindowWidth - lineWidth) * 0.5f, 300.0f, instrPixel, white);
}

void PauseScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.scancode == SDL_SCANCODE_P || event.key.scancode == SDL_SCANCODE_ESCAPE)
        {
            m_manager->PopScene();
        }
    }
}