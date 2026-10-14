#pragma once

#include "Component.hpp"

class GameObject;

class AIControllerComponent : public Component
{
    public:
        void SetTarget(GameObject *target) { m_target = target; }
        void FixedUpdate(float fixed_dt) override;

    private:
        GameObject *m_target{nullptr};

};