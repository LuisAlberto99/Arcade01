#include "TitleScene.hpp"
#include "SceneManager.hpp"
#include "GameplayScene.hpp"
#include "BitmapFont.hpp"
#include "GameConstants.hpp"

void TitleScene::Update(float dt)
{
    Scene::Update(dt);

    m_blinkTimer += dt;
    if (m_blinkTimer >= 0.5f)
    {
        m_blinkTimer = 0.0f;
        m_showPrompt = !m_showPrompt;
    }
}

void TitleScene::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);
    SDL_RenderClear(renderer);

    SDL_Color white{255, 255, 255, 255};
    //título "PONG"
    const float titlePixel = 11.0f;
    float titleWidth = BitmapFont::MeasureTextWidth("PONG", titlePixel);
    BitmapFont::DrawText(renderer, "PONG", (GameConstants::WindowWidth - titleWidth) * 0.5f, 100.0f, titlePixel, white);

    const float instrPixel = 5.0f;
    std::string line1 = "INICIAR";
    float line1Width = BitmapFont::MeasureTextWidth(line1, instrPixel);
    BitmapFont::DrawText(renderer, line1, (GameConstants::WindowWidth - line1Width) * 0.5f, 280.0f, instrPixel, white);

    std::string line2 = "PRECIONA LA TECLA ESPACIO";
    float line2Width = BitmapFont::MeasureTextWidth(line2, instrPixel);
    BitmapFont::DrawText(renderer, line2, (GameConstants::WindowWidth - line2Width) * 0.5f, 320.0f, instrPixel, white);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_FRect leftPaddle{60.0f, 400.0f, 20.0f, 80.0f};
    SDL_FRect rightPaddle{GameConstants::WindowWidth - 80.0f, 400.0f, 20.0f, 80.0f};
    SDL_FRect ball{GameConstants::WindowWidth * 0.5f - 8.0f, 430.0f, 16.0f, 16.0f};
    SDL_RenderFillRect(renderer, &leftPaddle);
    SDL_RenderFillRect(renderer, &rightPaddle);
    SDL_RenderFillRect(renderer, &ball);

    // Indicador parpadeante de "presiona una tecla"
    if (m_showPrompt)
    {
        SDL_FRect prompt{GameConstants::WindowWidth * 0.5f - 40.0f, 360.0f, 80.0f, 6.0f};
        SDL_RenderFillRect(renderer, &prompt);
    }
}

void TitleScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.scancode == SDL_SCANCODE_SPACE || event.key.scancode == SDL_SCANCODE_RETURN)
        {
            m_manager->ChangeScene(std::make_unique<GameplayScene>(m_manager));
        }
    }
}