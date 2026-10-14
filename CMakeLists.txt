cmake_minimum_required(VERSION 3.16)
project(proyecto01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
    set(SDL3_TARGET PkgConfig::SDL3)
else()
    set(SDL3_TARGET SDL3::SDL3)
endif()

add_executable(${PROJECT_NAME}
    src/main.cpp

    include/Vector2.hpp
    src/Vector2.cpp

    include/Component.hpp

    include/GameObject.hpp
    src/GameObject.cpp

    include/TransformComponent.hpp
    src/TransformComponent.cpp

    include/RectRenderComponent.hpp
    src/RectRenderComponent.cpp

    include/ColliderComponent.hpp
    src/ColliderComponent.cpp

    include/CollisionManager.hpp
    src/CollisionManager.cpp

    include/Scene.hpp
    src/Scene.cpp

    include/SceneManager.hpp
    src/SceneManager.cpp

    include/GameConstants.hpp

    include/PlayerControllerComponent.hpp
    src/PlayerControllerComponent.cpp

    include/AIControllerComponent.hpp
    src/AIControllerComponent.cpp

    include/BallComponent.hpp
    src/BallComponent.cpp

    include/GoalSensorComponent.hpp
    src/GoalSensorComponent.cpp

    include/SevenSegmentDisplay.hpp
    src/SevenSegmentDisplay.cpp

    include/BitmapFont.hpp
    src/BitmapFont.cpp

    include/TitleScene.hpp
    src/TitleScene.cpp

    include/GameplayScene.hpp
    src/GameplayScene.cpp

    include/PauseScene.hpp
    src/PauseScene.cpp

    include/GameOverScene.hpp
    src/GameOverScene.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include src)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_TARGET})