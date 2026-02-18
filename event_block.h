#ifndef EVENT_BLOCK_H
#define EVENT_BLOCK_H

#include "sprite.h"

struct MessageBus
{
    vector <string> pendingMessages;

    void broadcast (string msg)
    {
        pendingMessages.push_back (msg);
    }

    bool received (string msg)
    {
        for (int i = 0; i < (int)pendingMessages.size (); i++)
            if (pendingMessages [i] == msg)
            {
                pendingMessages.erase (pendingMessages.begin () + i);
                return true;
            }
        return false;
    }

    void clear ()
    {
        pendingMessages.clear ();
    }
};

bool onFlagClicked (vector <SDL_Event> &events, SDL_Rect flagBtn)
{
    for (auto &e : events)
        if (e.type == SDL_MOUSEBUTTONDOWN and e.button.button == SDL_BUTTON_LEFT)
        {
            int x, y;
            SDL_GetMouseState (&x, &y);
            if (x >= flagBtn.x and x <= flagBtn.x + flagBtn.w and
                y >= flagBtn.y and y <= flagBtn.y + flagBtn.h)
                return true;
        }
    return false;
}

bool onKeyPressed (vector <SDL_Event> &events, SDL_Keycode key)
{
    for (auto &e : events)
        if (e.type == SDL_KEYDOWN and e.key.keysym.sym == key)
            return true;
    return false;
}

bool onSpriteClicked (vector <SDL_Event> &events, Sprite &sprite)
{
    for (auto &e : events)
        if (e.type == SDL_MOUSEBUTTONDOWN and e.button.button == SDL_BUTTON_LEFT)
        {
            int x, y;
            SDL_GetMouseState (&x, &y);
            if (x >= sprite.img_rect.x and x <= sprite.img_rect.x + sprite.img_rect.w and
                y >= sprite.img_rect.y and y <= sprite.img_rect.y + sprite.img_rect.h)
                return true;
        }
    return false;
}

bool onBackdropSwitched (string current, string target)
{
    return current == target;
}

#endif //EVENT_BLOCK_H