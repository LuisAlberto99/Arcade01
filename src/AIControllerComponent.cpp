#include "AIControllerComponent.hpp"
#include "GameObject.hpp"
#include "TransformComponent.hpp"
#include "GameConstants.hpp"

void AIControllerComponent::FixedUpdate(float fixed_dt)
{
    if (!owner || !m_target) return;

    TransformComponent *transform = owner->GetComponent<TransformComponent>();
    TransformComponent *targetTransform = m_target->GetComponent<TransformComponent>();
    if (!transform || !targetTransform) return;

    float paddleCenterY = transform->position.y + (GameConstants::PaddleHeight * transform->scale.y) * 0.5f;
    float targetCenterY = targetTransform->position.y;

    float direction = 0.0f;
    const float deadZone = 9.0f; // evita que la paleta "tiemble" cuando ya está casi alineada
    if (targetCenterY < paddleCenterY - deadZone) direction = -1.0f;
    else if (targetCenterY > paddleCenterY + deadZone) direction = 1.0f;

    Vector2 velocity{0.0f, direction};
    
    // IA "básica": % de la velocidad del jugador.
    transform->Translate(velocity * (GameConstants::PaddleSpeed * 0.45f * fixed_dt));

    float maxY = GameConstants::WindowHeight - GameConstants::PaddleHeight * transform->scale.y;
    transform->position = transform->position.clamp(
        Vector2{transform->position.x, 0.0f},
        Vector2{transform->position.x, maxY});
}