#pragma once

#include "Scene.hpp"

class GameOverScene : public Scene
{
    public:
        GameOverScene(SceneManager *manager, bool playerWon, int playerScore, int aiScore)
            : Scene(manager, "GameOver"), m_playerWon(playerWon), m_playerScore(playerScore), m_aiScore(aiScore) {}

        void Render(SDL_Renderer *renderer) override;
        void HandleEvent(const SDL_Event &event) override;

    private:
        bool m_playerWon;
        int m_playerScore;
        int m_aiScore;

};