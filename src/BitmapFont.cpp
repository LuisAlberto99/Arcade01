#include "BitmapFont.hpp"
#include <unordered_map>

namespace
{
    // Cada glifo: 7 filas de 5 caracteres ('#' = relleno, '.' = vacío)
    using Glyph = const char (*)[6];

    constexpr char GLYPH_A[7][6] = {".###.", "#...#", "#...#", "#####", "#...#", "#...#", "#...#"};
    constexpr char GLYPH_C[7][6] = {".####", "#....", "#....", "#....", "#....", "#....", ".####"};
    constexpr char GLYPH_D[7][6] = {"####.", "#...#", "#...#", "#...#", "#...#", "#...#", "####."};
    constexpr char GLYPH_E[7][6] = {"#####", "#....", "#....", "####.", "#....", "#....", "#####"};
    constexpr char GLYPH_F[7][6] = {"#####", "#....", "#....", "####.", "#....", "#....", "#...."};
    constexpr char GLYPH_G[7][6] = {".####", "#....", "#....", "#.###", "#...#", "#...#", ".####"};
    constexpr char GLYPH_H[7][6] = {"#...#", "#...#", "#...#", "#####", "#...#", "#...#", "#...#"};
    constexpr char GLYPH_I[7][6] = {"#####", "..#..", "..#..", "..#..", "..#..", "..#..", "#####"};
    constexpr char GLYPH_L[7][6] = {"#....", "#....", "#....", "#....", "#....", "#....", "#####"};
    constexpr char GLYPH_M[7][6] = {"#...#", "##.##", "#.#.#", "#...#", "#...#", "#...#", "#...#"};
    constexpr char GLYPH_N[7][6] = {"#...#", "##..#", "#.#.#", "#..##", "#...#", "#...#", "#...#"};
    constexpr char GLYPH_O[7][6] = {".###.", "#...#", "#...#", "#...#", "#...#", "#...#", ".###."};
    constexpr char GLYPH_P[7][6] = {"####.", "#...#", "#...#", "####.", "#....", "#....", "#...."};
    constexpr char GLYPH_R[7][6] = {"####.", "#...#", "#...#", "####.", "#.#..", "#..#.", "#...#"};
    constexpr char GLYPH_S[7][6] = {".####", "#....", "#....", ".###.", "....#", "....#", "####."};
    constexpr char GLYPH_T[7][6] = {"#####", "..#..", "..#..", "..#..", "..#..", "..#..", "..#.."};
    constexpr char GLYPH_U[7][6] = {"#...#", "#...#", "#...#", "#...#", "#...#", "#...#", ".###."};
    constexpr char GLYPH_V[7][6] = {"#...#", "#...#", "#...#", "#...#", "#...#", ".#.#.", "..#.."};
    constexpr char GLYPH_SPACE[7][6] = {".....", ".....", ".....", ".....", ".....", ".....", "....."};
    constexpr char GLYPH_COLON[7][6] = {".....", "..#..", ".....", ".....", ".....", "..#..", "....."};

    const Glyph *FindGlyph(char c)
    {
        static const std::unordered_map<char, Glyph> glyphs = {
            {'A', GLYPH_A}, {'C', GLYPH_C}, {'D', GLYPH_D}, {'E', GLYPH_E}, {'F', GLYPH_F},
            {'G', GLYPH_G}, {'H', GLYPH_H}, {'I', GLYPH_I}, {'L', GLYPH_L}, {'M', GLYPH_M},
            {'N', GLYPH_N}, {'O', GLYPH_O}, {'P', GLYPH_P}, {'R', GLYPH_R}, {'S', GLYPH_S},
            {'T', GLYPH_T}, {'U', GLYPH_U}, {'V', GLYPH_V}, {' ', GLYPH_SPACE}, {':', GLYPH_COLON},
        };
        auto it = glyphs.find(c);
        return it != glyphs.end() ? &it->second : nullptr;
    }
}

void BitmapFont::DrawChar(SDL_Renderer *renderer, char c, float x, float y, float pixelSize, SDL_Color color)
{
    const Glyph *glyph = FindGlyph(c);
    if (!glyph) return;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int row = 0; row < 7; ++row)
    {
        for (int col = 0; col < 5; ++col)
        {
            if ((*glyph)[row][col] == '#')
            {
                SDL_FRect pixel{x + col * pixelSize, y + row * pixelSize, pixelSize, pixelSize};
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
}

void BitmapFont::DrawText(SDL_Renderer *renderer, const std::string &text, float x, float y, float pixelSize, SDL_Color color)
{
    float cursorX = x;
    float charWidth = pixelSize * 6.0f; // 5 columnas + 1 de espacio entre letras

    for (char c : text)
    {
        DrawChar(renderer, c, cursorX, y, pixelSize, color);
        cursorX += charWidth;
    }
}

float BitmapFont::MeasureTextWidth(const std::string &text, float pixelSize)
{
    if (text.empty()) return 0.0f;
    float charWidth = pixelSize * 6.0f;
    return charWidth * static_cast<float>(text.size()) - pixelSize; // sin espacio sobrante al final
}