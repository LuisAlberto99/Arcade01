#include "BallComponent.hpp"
#include "GameObject.hpp"
#include "TransformComponent.hpp"
#include "ColliderComponent.hpp"
#include "GameConstants.hpp"
#include <numbers>
#include <cmath>

void BallComponent::Init()
{
    ResetToCenter(-1.0f);
}

void BallComponent::FixedUpdate(float fixed_dt)
{
    if (!owner) return;
    TransformComponent *transform = owner->GetComponent<TransformComponent>();
    if (!transform) return;

    transform->Translate(m_velocity * fixed_dt);

    // Rebote contra los bordes superior e inferior de la ventana
    float minY = 0.0f;
    float maxY = GameConstants::WindowHeight - GameConstants::BallSize * transform->scale.y;

    if (transform->position.y < minY)
    {
        transform->position.y = minY;
        m_velocity.y = -m_velocity.y;
    }
    else if (transform->position.y > maxY)
    {
        transform->position.y = maxY;
        m_velocity.y = -m_velocity.y;
    }
}

void BallComponent::OnCollision(GameObject *other)
{
    if (!other) return;

    if (other->GetTag() == "PlayerPaddle")
    {
        BounceOffPaddle(other, 1.0f); // rebota hacia la derecha, hacia la IA
    }
    else if (other->GetTag() == "AIPaddle")
    {
        BounceOffPaddle(other, -1.0f); // rebota hacia la izquierda, hacia el jugador
    }
}

void BallComponent::BounceOffPaddle(GameObject *paddle, float direction)
{
    if (!owner) return;

    ColliderComponent *ownCollider = owner->GetComponent<ColliderComponent>();
    ColliderComponent *paddleCollider = paddle->GetComponent<ColliderComponent>();
    TransformComponent *transform = owner->GetComponent<TransformComponent>();
    if (!ownCollider || !paddleCollider || !transform) return;

    SDL_FRect ballBounds = ownCollider->GetWorldBounds();
    SDL_FRect paddleBounds = paddleCollider->GetWorldBounds();

    // Punto de contacto relativo: -1.0 (borde superior de la paleta) a +1.0 (borde inferior)
    float ballCenterY = ballBounds.y + ballBounds.h * 0.5f;
    float paddleCenterY = paddleBounds.y + paddleBounds.h * 0.5f;
    float relativeIntersectY = (ballCenterY - paddleCenterY) / (paddleBounds.h * 0.5f);
    relativeIntersectY = relativeIntersectY < -1.0f ? -1.0f : (relativeIntersectY > 1.0f ? 1.0f : relativeIntersectY);

    float maxAngleRad = GameConstants::BallMaxBounceAngleDeg * std::numbers::pi_v<float> / 180.0f;
    float bounceAngle = relativeIntersectY * maxAngleRad;

    float speed = m_velocity.length() * GameConstants::BallSpeedUpFactor;
    if (speed > GameConstants::BallMaxSpeed)
    {
        speed = GameConstants::BallMaxSpeed;
    }

    m_velocity.x = direction * speed * std::cos(bounceAngle);
    m_velocity.y = speed * std::sin(bounceAngle);

    // Reposicionamos la pelota justo afuera de la paleta para que la misma colisión
    // no se detecte varias veces seguidas mientras ambas cajas sigan traslapadas.
    if (direction > 0.0f)
    {
        transform->position.x = paddleBounds.x + paddleBounds.w;
    }
    else
    {
        transform->position.x = paddleBounds.x - ballBounds.w;
    }
}

void BallComponent::ResetToCenter(float serveDirection)
{
    if (!owner) return;
    TransformComponent *transform = owner->GetComponent<TransformComponent>();
    if (!transform) return;

    transform->position.x = (GameConstants::WindowWidth - GameConstants::BallSize * transform->scale.x) * 0.5f;
    transform->position.y = (GameConstants::WindowHeight - GameConstants::BallSize * transform->scale.y) * 0.5f;

    m_velocity = Vector2{serveDirection * GameConstants::BallInitialSpeed, 0.0f};
}