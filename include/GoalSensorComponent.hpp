#pragma once

#include "Component.hpp"

class GameplayScene;

enum class GoalSide
{
    Left,
    Right
};

class GoalSensorComponent : public Component
{
    public:
        GoalSensorComponent(GameplayScene *scene, GoalSide side)
            : m_scene(scene), m_side(side) {}

        void OnCollision(GameObject *other) override;

    private:
        GameplayScene *m_scene{nullptr};
        GoalSide m_side;
};