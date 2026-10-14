#include "PlayerControllerComponent.hpp"
#include "GameObject.hpp"
#include "TransformComponent.hpp"
#include "GameConstants.hpp"
#include <SDL3/SDL.h>

void PlayerControllerComponent::FixedUpdate(float fixed_dt)
{
    if (!owner) return;
    TransformComponent *transform = owner->GetComponent<TransformComponent>();
    if (!transform) return;

    const bool *keys = SDL_GetKeyboardState(nullptr);

    float direction = 0.0f;
    if (keys[SDL_SCANCODE_UP])   direction -= 1.0f;
    if (keys[SDL_SCANCODE_DOWN]) direction += 1.0f;

    Vector2 velocity{0.0f, direction};
    if (velocity.length_squared() > 0.0f)
    {
        velocity = velocity.normalized();
    }

    transform->Translate(velocity * (GameConstants::PaddleSpeed * fixed_dt));

    float maxY = GameConstants::WindowHeight - GameConstants::PaddleHeight * transform->scale.y;
    transform->position = transform->position.clamp(
        Vector2{transform->position.x, 0.0f},
        Vector2{transform->position.x, maxY});
}