#pragma once

struct TextTexture
{
    TextTexture(struct SDL_Texture* tex, int w, int h)
        : tex(tex), texWidth(w), texHeight(h){}
    struct SDL_Texture* tex;
    int texWidth;
    int texHeight;
};