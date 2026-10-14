#pragma once

#include "Scene.hpp"
#include "CollisionManager.hpp"

enum class GoalSide;

class GameplayScene : public Scene
{
public:
    explicit GameplayScene(SceneManager *manager);

    void Init() override;
    void FixedUpdate(float fixed_dt) override;
    void Render(SDL_Renderer *renderer) override;
    void HandleEvent(const SDL_Event &event) override;

    void OnGoalScored(GoalSide side);

private:
    CollisionManager m_collisionManager;
    GameObject *m_ball{nullptr};

    int m_playerScore{0};
    int m_aiScore{0};

    bool m_debugDraw{false};
    void ToggleDebugDraw() { m_debugDraw = !m_debugDraw; }
};