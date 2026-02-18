#ifndef VARIABLE_BLOCK_H
#define VARIABLE_BLOCK_H

#include "sprite.h"

struct Variable
{
    string name;
    double value = 0;
    bool visible = true;
};

struct VarManager
{
    vector <Variable> vars;

    bool exists (string name)
    {
        for (auto &v : vars)
            if (v.name == name) return true;
        return false;
    }

    bool defineVar (string name)
    {
        if (exists (name)) return false;
        Variable v;
        v.name = name;
        vars.push_back (v);
        return true;
    }

    void setVar (string name, double val)
    {
        for (auto &v : vars)
            if (v.name == name) { v.value = val; return; }
    }

    void changeVar (string name, double delta)
    {
        for (auto &v : vars)
            if (v.name == name) { v.value += delta; return; }
    }

    double getVar (string name)
    {
        for (auto &v : vars)
            if (v.name == name) return v.value;
        return 0;
    }

    void showVar (string name)
    {
        for (auto &v : vars)
            if (v.name == name) { v.visible = true; return; }
    }

    void hideVar (string name)
    {
        for (auto &v : vars)
            if (v.name == name) { v.visible = false; return; }
    }

    void drawAll (SDL_Renderer *m_renderer, TTF_Font *font)
    {
        int yoff = 225;
        for (auto &v : vars)
        {
            if (!v.visible) continue;
            string txt = v.name + " = " + to_string ((int)round (v.value));
            if (font)
            {
                SDL_Color c = {255, 140, 0};
                SDL_Surface *s = TTF_RenderText_Blended (font, txt.c_str (), c);
                SDL_Texture *t = SDL_CreateTextureFromSurface (m_renderer, s);
                SDL_Rect bg = {875, yoff - 2, s -> w + 4, s -> h + 4};
                SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
                SDL_RenderFillRect (m_renderer, &bg);
                SDL_Rect tr = {877, yoff, s -> w, s -> h};
                SDL_RenderCopy (m_renderer, t, nullptr, &tr);
                SDL_DestroyTexture (t);
                SDL_FreeSurface (s);
                yoff += 30;
            }
        }
    }
};

#endif //VARIABLE_BLOCK_H