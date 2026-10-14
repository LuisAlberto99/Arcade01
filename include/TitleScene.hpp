#pragma once

#include "Scene.hpp"

class TitleScene : public Scene
{
    public:
        explicit TitleScene(SceneManager *manager) : Scene(manager, "Title") {}

        void Update(float dt) override;
        void Render(SDL_Renderer *renderer) override;
        void HandleEvent(const SDL_Event &event) override;

    private:
        float m_blinkTimer{0.0f};
        bool m_showPrompt{true};
};
