
#ifndef SPRITE_H
#define SPRITE_H

#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <SDL2/SDL_mixer.h>

using namespace std;

struct Stage
{
    vector <string> backdropsName;
    vector <SDL_Texture*> backdrops;
    int curBackdropNum;
};

struct Sprite
{
    string mainName;
    SDL_Texture *m_img = nullptr;
    SDL_Rect img_rect;
    SDL_Rect img_src;
    Mix_Chunk *m_sound = nullptr;
    int vol = 128;
    bool visible = true;
    int w, h;
    int x, y;
    double direction = 0.0;
    vector <string> costumesName;
    vector <SDL_Texture*> costumes;
    int curCostumeNum;
    double scale = 100;
    int a = 255;
    int layer = 0;

    void draw (SDL_Renderer* m_renderer)
    {
        if (!visible) {return;}
        SDL_QueryTexture (m_img, NULL, NULL, &w, &h);
        img_rect.x = x - w / 6;
        img_rect.y = y - h / 6;
        img_rect.w = w / 3;
        img_rect.h = h / 3;
        img_src.x = 0;
        img_src.y = 0;
        img_src.w = w / 1;
        img_src.h = h / 1;
        // copy the texture to the rendering context
        SDL_RenderCopy (m_renderer, m_img, &img_src, &img_rect);
    }
};

#endif //SPRITE_H