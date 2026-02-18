#ifndef SENSING_BLOCK_H
#define SENSING_BLOCK_H

#include "sprite.h"

bool touchingSprite (Sprite &a, Sprite &b)
{
    SDL_Rect ra = {a.img_rect.x, a.img_rect.y, a.img_rect.w, a.img_rect.h};
    SDL_Rect rb = {b.img_rect.x, b.img_rect.y, b.img_rect.w, b.img_rect.h};
    return SDL_HasIntersection (&ra, &rb);
}

bool touchingEdge (Sprite &sprite)
{
    return (sprite.x - sprite.img_rect.w / 2 <= 870 or
            sprite.x + sprite.img_rect.w / 2 >= 1490 or
            sprite.y - sprite.img_rect.h / 2 <= 120 or
            sprite.y + sprite.img_rect.h / 2 >= 600);
}

double distanceTo (Sprite &a, Sprite &b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt (dx * dx + dy * dy);
}

double distanceToMouse (Sprite &sprite)
{
    int mx, my;
    SDL_GetMouseState (&mx, &my);
    double dx = sprite.x - mx;
    double dy = sprite.y - my;
    return sqrt (dx * dx + dy * dy);
}

bool keyPressed (SDL_Keycode key)
{
    const Uint8 *state = SDL_GetKeyboardState (nullptr);
    SDL_Scancode sc = SDL_GetScancodeFromKey (key);
    return state [sc];
}

bool mouseDown ()
{
    return SDL_GetMouseState (nullptr, nullptr) & SDL_BUTTON (SDL_BUTTON_LEFT);
}

int mouseX ()
{
    int x;
    SDL_GetMouseState (&x, nullptr);
    return x;
}

int mouseY ()
{
    int y;
    SDL_GetMouseState (nullptr, &y);
    return y;
}

bool touchingMouse (Sprite &sprite)
{
    int mx, my;
    SDL_GetMouseState (&mx, &my);
    return (mx >= sprite.img_rect.x and mx <= sprite.img_rect.x + sprite.img_rect.w and
            my >= sprite.img_rect.y and my <= sprite.img_rect.y + sprite.img_rect.h);
}

void startDrag (Sprite &sprite, SDL_Event &event, bool &dragging, int &offx, int &offy)
{
    if (event.type == SDL_MOUSEBUTTONDOWN and event.button.button == SDL_BUTTON_LEFT)
    {
        int mx, my;
        SDL_GetMouseState (&mx, &my);
        if (mx >= sprite.img_rect.x and mx <= sprite.img_rect.x + sprite.img_rect.w and
            my >= sprite.img_rect.y and my <= sprite.img_rect.y + sprite.img_rect.h)
        {
            dragging = true;
            offx = mx - sprite.x;
            offy = my - sprite.y;
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP and event.button.button == SDL_BUTTON_LEFT)
        dragging = false;
    if (dragging and event.type == SDL_MOUSEMOTION)
    {
        int mx, my;
        SDL_GetMouseState (&mx, &my);
        sprite.oldx = sprite.x;
        sprite.oldy = sprite.y;
        sprite.x = mx - offx;
        sprite.y = my - offy;
    }
}

double timerValue (Uint32 startTick)
{
    return (SDL_GetTicks () - startTick) / 1000.0;
}

void resetTimer (Uint32 &startTick)
{
    startTick = SDL_GetTicks ();
}

string askAndWait (string question, SDL_Renderer *m_renderer, TTF_Font *font)
{
    string answer;
    bool done = false;
    SDL_StartTextInput ();
    while (!done)
    {
        SDL_Event ev;
        SDL_SetRenderDrawColor (m_renderer, 255, 255, 255, 255);
        SDL_Rect bubble = {870, 560, 620, 40};
        SDL_RenderFillRect (m_renderer, &bubble);
        SDL_SetRenderDrawColor (m_renderer, 100, 100, 100, 255);
        SDL_RenderDrawRect (m_renderer, &bubble);
        if (font)
        {
            SDL_Color qc = {0, 0, 0};
            SDL_Surface *qs = TTF_RenderText_Blended (font, question.c_str (), qc);
            SDL_Texture *qt = SDL_CreateTextureFromSurface (m_renderer, qs);
            SDL_Rect qr = {875, 565, qs -> w, qs -> h};
            SDL_RenderCopy (m_renderer, qt, nullptr, &qr);
            SDL_DestroyTexture (qt);
            SDL_FreeSurface (qs);
            if (!answer.empty ())
            {
                SDL_Surface *as = TTF_RenderText_Blended (font, answer.c_str (), qc);
                SDL_Texture *at = SDL_CreateTextureFromSurface (m_renderer, as);
                SDL_Rect ar = {875 + qs -> w + 10, 565, as -> w, as -> h};
                SDL_RenderCopy (m_renderer, at, nullptr, &ar);
                SDL_DestroyTexture (at);
                SDL_FreeSurface (as);
            }
        }
        SDL_RenderPresent (m_renderer);
        while (SDL_PollEvent (&ev))
        {
            if (ev.type == SDL_TEXTINPUT)
                answer += ev.text.text;
            if (ev.type == SDL_KEYDOWN)
            {
                if (ev.key.keysym.sym == SDLK_RETURN)
                    done = true;
                else if (ev.key.keysym.sym == SDLK_BACKSPACE and !answer.empty ())
                    answer.pop_back ();
            }
        }
    }
    SDL_StopTextInput ();
    return answer;
}

#endif //SENSING_BLOCK_H