#pragma once

#include "Component.hpp"

class PlayerControllerComponent : public Component
{
    public:
        void FixedUpdate(float fixed_dt) override;
};