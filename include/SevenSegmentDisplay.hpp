#pragma once

#include <SDL3/SDL.h>

namespace SevenSegmentDisplay
{
    void DrawDigit(SDL_Renderer *renderer, int digit, float x, float y, float width, float height, SDL_Color color);
    void DrawNumber(SDL_Renderer *renderer, int number, float x, float y, float width, float height, SDL_Color color);
}