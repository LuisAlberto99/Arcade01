#include "GameOverScene.hpp"
#include "SceneManager.hpp"
#include "TitleScene.hpp"
#include "GameplayScene.hpp"
#include "BitmapFont.hpp"
#include "SevenSegmentDisplay.hpp"
#include "GameConstants.hpp"

void GameOverScene::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);
    SDL_RenderClear(renderer);

    SDL_Color white{255, 255, 255, 255};

    const float titlePixel = 8.0f;
    std::string title = m_playerWon ? "GANASTE" : "PERDISTE";
    float titleWidth = BitmapFont::MeasureTextWidth(title, titlePixel);
    BitmapFont::DrawText(renderer, title, (GameConstants::WindowWidth - titleWidth) * 0.5f, 150.0f, titlePixel, white);

    SevenSegmentDisplay::DrawNumber(renderer, m_playerScore, GameConstants::WindowWidth * 0.5f - 60.0f, 260.0f, 18.0f, 32.0f, white);
    SevenSegmentDisplay::DrawNumber(renderer, m_aiScore, GameConstants::WindowWidth * 0.5f + 20.0f, 260.0f, 18.0f, 32.0f, white);

    const float instrPixel = 4.0f;
    std::string line = "R: REINICIAR   M: MENU";
    float lineWidth = BitmapFont::MeasureTextWidth(line, instrPixel);
    BitmapFont::DrawText(renderer, line, (GameConstants::WindowWidth - lineWidth) * 0.5f, 340.0f, instrPixel, white);
}

void GameOverScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.scancode == SDL_SCANCODE_R)
        {
            m_manager->ChangeScene(std::make_unique<GameplayScene>(m_manager));
        }
        else if (event.key.scancode == SDL_SCANCODE_M || event.key.scancode == SDL_SCANCODE_ESCAPE)
        {
            m_manager->ChangeScene(std::make_unique<TitleScene>(m_manager));
        }
    }
}