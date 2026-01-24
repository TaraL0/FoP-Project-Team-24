
#ifndef SPRITE_H
#define SPRITE_H

#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>

struct Sprite
{
    SDL_Texture *m_img = nullptr;
    SDL_Rect img_rect;
    SDL_Rect img_src;
    int direction;
    bool visible;
    int w, h;
    double angle = 0.0;
    void draw (SDL_Renderer* m_renderer)
    {
        SDL_QueryTexture (m_img, NULL, NULL, &w, &h);
        img_rect.x = 400;
        img_rect.y = 200;
        img_rect.w = w;
        img_rect.h = h;
        img_src.x = 0;
        img_src.y = 0;
        img_src.w = w / 1;
        img_src.h = h / 1;
        // copy the texture to the rendering context
        SDL_RenderCopy (m_renderer, m_img, &img_src, &img_rect);
        SDL_RenderPresent (m_renderer);
    }
};

#endif //SPRITE_H