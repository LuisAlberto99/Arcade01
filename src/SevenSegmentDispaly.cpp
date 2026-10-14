#include "SevenSegmentDisplay.hpp"

namespace
{
    // Segmentos encendidos por dígito, en orden a-b-c-d-e-f-g
    // (a: arriba, b: arriba-derecha, c: abajo-derecha, d: abajo,
    //  e: abajo-izquierda, f: arriba-izquierda, g: medio)
    constexpr bool kDigitSegments[10][7] = {
        {true, true, true, true, true, true, false},     // 0
        {false, true, true, false, false, false, false}, // 1
        {true, true, false, true, true, false, true},    // 2
        {true, true, true, true, false, false, true},    // 3
        {false, true, true, false, false, true, true},   // 4
        {true, false, true, true, false, true, true},    // 5
        {true, false, true, true, true, true, true},     // 6
        {true, true, true, false, false, false, false},  // 7
        {true, true, true, true, true, true, true},      // 8
        {true, true, true, true, false, true, true},     // 9
    };
}

void SevenSegmentDisplay::DrawDigit(SDL_Renderer *renderer, int digit, float x, float y, float width, float height, SDL_Color color)
{
    if (digit < 0 || digit > 9) return;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    float t = height * 0.12f; // grosor de cada segmento
    const bool *segments = kDigitSegments[digit];

    SDL_FRect segmentRects[7] = {
        {x + t, y, width - 2 * t, t},                           // a: superior
        {x + width - t, y, t, height / 2.0f},                   // b: superior derecha
        {x + width - t, y + height / 2.0f, t, height / 2.0f},   // c: inferior derecha
        {x + t, y + height - t, width - 2 * t, t},               // d: inferior
        {x, y + height / 2.0f, t, height / 2.0f},                // e: inferior izquierda
        {x, y, t, height / 2.0f},                                // f: superior izquierda
        {x + t, y + height / 2.0f - t / 2.0f, width - 2 * t, t}, // g: media
    };

    for (int i = 0; i < 7; ++i)
    {
        if (segments[i])
        {
            SDL_RenderFillRect(renderer, &segmentRects[i]);
        }
    }
}

void SevenSegmentDisplay::DrawNumber(SDL_Renderer *renderer, int number, float x, float y, float width, float height, SDL_Color color)
{
    if (number < 0) number = -number;

    int digits[16];
    int count = 0;
    if (number == 0)
    {
        digits[count++] = 0;
    }
    while (number > 0 && count < 16)
    {
        digits[count++] = number % 10;
        number /= 10;
    }

    float spacing = width * 0.3f;
    float cursorX = x;

    // digits[] quedó en orden inverso (unidades primero); lo recorremos al revés
    // para dibujar de izquierda a derecha en el orden correcto.
    for (int i = count - 1; i >= 0; --i)
    {
        DrawDigit(renderer, digits[i], cursorX, y, width, height, color);
        cursorX += width + spacing;
    }
}