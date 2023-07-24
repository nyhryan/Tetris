#pragma once
#include <cstdint>
#include <SDL.h>



struct Color
{
    using color32 = uint32_t;

    static constexpr color32 BLACK = 0x000000FF;
    static constexpr color32 CYAN = 0x00FFFFFF;     // I block - index 1
    static constexpr color32 BLUE = 0x0026FFFF;     // J block - index 2
    static constexpr color32 ORANGE = 0xFF7200FF;   // L block - index 3
    static constexpr color32 YELLOW = 0xFFD800FF;   // O block - index 4
    static constexpr color32 GREEN = 0x00FF21FF;    // S block - index 5
    static constexpr color32 RED = 0xFF0000FF;      // Z block - index 6
    static constexpr color32 MAGENTA = 0xB200FFFF;  // T block - index 7
    static constexpr color32 PINK = 0xF917F1FF;
    static constexpr color32 WHITE = 0xFFFFFFFF;
    static constexpr color32 GRAY = 0x505050FF;
    static constexpr color32 LIGHT_GRAY = 0xBBBBBBFF;
    static constexpr color32 DARK_GRAY = 0x696969FF;


    static constexpr color32 Colors[9] = {
        BLACK,
        CYAN,
        BLUE,
        ORANGE,
        YELLOW,
        GREEN,
        RED,
        MAGENTA,
        WHITE
    };

    static const unsigned char GetRed(color32 rgba)
    {
        return rgba >> 24;
    }
    static const unsigned char GetGreen(color32 rgba)
    {
        return (rgba >> 16) & 0xFF;
    }
    static const unsigned char GetBlue(color32 rgba)
    {
        return (rgba >> 8) & 0xFF;
    }
    static const unsigned char GetAlpha(color32 rgba)
    {
        return rgba & 0xFF;
    }

    static void SetDrawColor(SDL_Renderer* renderer, color32 rgba)
    {
        SDL_SetRenderDrawColor(
            renderer,
            Color::GetRed(rgba),
            Color::GetGreen(rgba),
            Color::GetBlue(rgba),
            Color::GetAlpha(rgba));
    }
};
