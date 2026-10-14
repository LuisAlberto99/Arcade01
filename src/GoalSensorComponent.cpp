#include "GoalSensorComponent.hpp"
#include "GameObject.hpp"
#include "GameplayScene.hpp"

void GoalSensorComponent::OnCollision(GameObject *other)
{
    if (!other || !m_scene) return;

    if (other->GetTag() == "Ball")
    {
        m_scene->OnGoalScored(m_side);
    }
}