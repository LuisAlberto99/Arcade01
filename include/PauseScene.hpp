#pragma once

#include "Scene.hpp"

class PauseScene : public Scene
{
    public:
        explicit PauseScene(SceneManager *manager) : Scene(manager, "Pause") {}

        void Render(SDL_Renderer *renderer) override;
        void HandleEvent(const SDL_Event &event) override;

};