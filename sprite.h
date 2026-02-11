
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
    SDL_Rect img_src;
    int curBackdropNum;

    void draw (SDL_Renderer* m_renderer, SDL_Rect mainStage)
    {
        if (backdrops.empty ()) return;;
        SDL_RenderCopy (m_renderer, backdrops [curBackdropNum], nullptr, &mainStage);
    }
};

struct Sprite
{
    string mainName;
    SDL_Rect img_rect;
    Mix_Chunk *m_sound = nullptr;
    double vol = 128.0;
    bool visible = true;
    int x, y;
    double direction = 0.0;
    vector <string> costumesName;
    vector <SDL_Texture*> costumes;
    vector <int> costumesWidth;
    vector <int> costumesHeight;
    int curCostumeNum = 0;
    double scale = 100;
    int a = 255;
    int layer;
    int layerBeforeChange;

    void costumeSetup (string name)
    {
        int w, h;
        costumesName.emplace_back (name);
        SDL_QueryTexture (costumes [curCostumeNum], nullptr, nullptr, &w, &h);
        costumesWidth.emplace_back (w);
        costumesHeight.emplace_back (h);
    }

    void setup ()
    {
        double var = scale / 300 ;
        img_rect.x = x - costumesWidth [curCostumeNum] * var / 2;
        img_rect.y = y - costumesHeight [curCostumeNum] * var / 2;
        img_rect.w = costumesWidth [curCostumeNum] * var;
        img_rect.h = costumesHeight [curCostumeNum] * var;
    }

    void draw (SDL_Renderer* m_renderer)
    {
        if (costumes.empty ()) return;
        if (!visible) {return;}
        setup ();
        SDL_RenderCopyEx (m_renderer, costumes [curCostumeNum], nullptr, &img_rect, direction, nullptr, SDL_FLIP_NONE);
    }
};

#endif //SPRITE_H