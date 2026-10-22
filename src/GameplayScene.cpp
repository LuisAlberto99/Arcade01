#include "GameplayScene.hpp"
#include "SceneManager.hpp"
#include "GameObject.hpp"
#include "TransformComponent.hpp"
#include "RectRenderComponent.hpp"
#include "ColliderComponent.hpp"
#include "PlayerControllerComponent.hpp"
#include "AIControllerComponent.hpp"
#include "BallComponent.hpp"
#include "GoalSensorComponent.hpp"
#include "PauseScene.hpp"
#include "GameOverScene.hpp"
#include "SevenSegmentDisplay.hpp"
#include "GameConstants.hpp"

GameplayScene::GameplayScene(SceneManager *manager)
    : Scene(manager, "Gameplay"), m_collisionManager(&m_entities)
{
}

void GameplayScene::Init()
{
    // Paleta del jugador (izquierda)
    GameObject *player = CreateGameObjects("PlayerPaddle");
    player->AddComponent<TransformComponent>(Vector2{40.0f, (GameConstants::WindowHeight - GameConstants::PaddleHeight) * 0.5f});
    player->AddComponent<RectRenderComponent>(
        Vector2{GameConstants::PaddleWidth, GameConstants::PaddleHeight},
        GameConstants::PlayerColor);
    player->AddComponent<ColliderComponent>(Vector2{GameConstants::PaddleWidth, GameConstants::PaddleHeight});
    player->AddComponent<PlayerControllerComponent>();

    // Paleta de la IA (derecha)
    GameObject *ai = CreateGameObjects("AIPaddle");
    ai->AddComponent<TransformComponent>(Vector2{
        GameConstants::WindowWidth - 40.0f - GameConstants::PaddleWidth,
        (GameConstants::WindowHeight - GameConstants::PaddleHeight) * 0.5f});
    ai->AddComponent<RectRenderComponent>(
        Vector2{GameConstants::PaddleWidth, GameConstants::PaddleHeight},
        GameConstants::AIColor);
    ai->AddComponent<ColliderComponent>(Vector2{GameConstants::PaddleWidth, GameConstants::PaddleHeight});
    AIControllerComponent *aiController = ai->AddComponent<AIControllerComponent>();

    // Pelota
    GameObject *ball = CreateGameObjects("Ball");
    ball->AddComponent<TransformComponent>(Vector2{0.0f, 0.0f});
    ball->AddComponent<RectRenderComponent>(
        Vector2{GameConstants::BallSize, GameConstants::BallSize},
        GameConstants::BallColor);
    ball->AddComponent<ColliderComponent>(Vector2{GameConstants::BallSize, GameConstants::BallSize});
    ball->AddComponent<BallComponent>();
    m_ball = ball;

    aiController->SetTarget(m_ball);

    // Sensores de anotación, justo fuera de cada borde lateral
    GameObject *leftGoal = CreateGameObjects("LeftGoalSensor");
    leftGoal->AddComponent<TransformComponent>(Vector2{-GameConstants::GoalSensorWidth, 0.0f});
    ColliderComponent *leftCollider = leftGoal->AddComponent<ColliderComponent>(
        Vector2{GameConstants::GoalSensorWidth, GameConstants::WindowHeight});
    leftCollider->is_trigger = true;
    leftGoal->AddComponent<GoalSensorComponent>(this, GoalSide::Left);

    GameObject *rightGoal = CreateGameObjects("RightGoalSensor");
    rightGoal->AddComponent<TransformComponent>(Vector2{GameConstants::WindowWidth, 0.0f});
    ColliderComponent *rightCollider = rightGoal->AddComponent<ColliderComponent>(
        Vector2{GameConstants::GoalSensorWidth, GameConstants::WindowHeight});
    rightCollider->is_trigger = true;
    rightGoal->AddComponent<GoalSensorComponent>(this, GoalSide::Right);
}

void GameplayScene::FixedUpdate(float fixed_dt)
{
    Scene::FixedUpdate(fixed_dt);       // mueve paletas y pelota
    m_collisionManager.CheckCollisions(); // detecta e informa colisiones con las posiciones ya actualizadas
}

void GameplayScene::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);
    SDL_RenderClear(renderer);

    // Línea discontinua al centro de la cancha
    SDL_SetRenderDrawColor(renderer, 90, 90, 100, 255);
    for (float y = 0.0f; y < GameConstants::WindowHeight; y += 30.0f)
    {
        SDL_FRect dash{GameConstants::WindowWidth * 0.5f - 2.0f, y, 4.0f, 16.0f};
        SDL_RenderFillRect(renderer, &dash);
    }

    Scene::Render(renderer); // dibuja paletas y pelota vía RectRenderComponent

    SDL_Color white{255, 255, 255, 255};
    SevenSegmentDisplay::DrawNumber(renderer, m_playerScore, GameConstants::WindowWidth * 0.25f, 20.0f, 20.0f, 36.0f, white);
    SevenSegmentDisplay::DrawNumber(renderer, m_aiScore, GameConstants::WindowWidth * 0.75f - 20.0f, 20.0f, 20.0f, 36.0f, white);

    if (m_debugDraw)
    {
        for (auto &entity : m_entities)
        {
            if (auto *collider = entity->GetComponent<ColliderComponent>())
            {
                collider->RenderDebug(renderer);
            }
        }
    }
}

void GameplayScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.scancode == SDL_SCANCODE_F1)
        {
            ToggleDebugDraw();
        }
        else if (event.key.scancode == SDL_SCANCODE_P || event.key.scancode == SDL_SCANCODE_ESCAPE)
        {
            m_manager->PushScene(std::make_unique<PauseScene>(m_manager));
        }
    }
}

void GameplayScene::OnGoalScored(GoalSide side)
{
    if (side == GoalSide::Left)
    {
        m_aiScore++; // la pelota entró por la izquierda: anota la IA
    }
    else
    {
        m_playerScore++; // entró por la derecha: anota el jugador
    }

    if (m_playerScore >= GameConstants::WinningScore || m_aiScore >= GameConstants::WinningScore)//Se revisa si hay un ganador.
    {
        bool playerWon = m_playerScore > m_aiScore;
        m_manager->ChangeScene(std::make_unique<GameOverScene>(m_manager, playerWon, m_playerScore, m_aiScore));
    }

    if (auto *ball = m_ball->GetComponent<BallComponent>())
    {
        float serveDirection = (side == GoalSide::Left) ? 1.0f : -1.0f; //Se regresa la pelota al centro y va hacia el que anota el goal.
        ball->ResetToCenter(serveDirection);
    }
}