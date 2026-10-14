#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace BitmapFont
{
    void DrawChar(SDL_Renderer *renderer, char c, float x, float y, float pixelSize, SDL_Color color);
    void DrawText(SDL_Renderer *renderer, const std::string &text, float x, float y, float pixelSize, SDL_Color color);

    float MeasureTextWidth(const std::string &text, float pixelSize);
}