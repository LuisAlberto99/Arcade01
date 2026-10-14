#pragma once

#include "Component.hpp"
#include "Vector2.hpp"

class BallComponent : public Component
{
public:
    void Init() override;
    void FixedUpdate(float fixed_dt) override;
    void OnCollision(GameObject *other) override;

    // Reposiciona la pelota al centro de la ventana y la sirve hacia un lado.
    // serveDirection: +1.0f sirve hacia la derecha (paleta IA), -1.0f hacia la izquierda (jugador).
    void ResetToCenter(float serveDirection);

private:
    Vector2 m_velocity{0.0f, 0.0f};

    void BounceOffPaddle(GameObject *paddle, float direction);
};