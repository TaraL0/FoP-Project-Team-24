#ifndef CONTROL_BLOCK_H
#define CONTROL_BLOCK_H

#include "sprite.h"

bool waitUntil (bool condition)
{
    return condition;
}

void waitSeconds (double seconds, SDL_Renderer *m_renderer)
{
    SDL_RenderPresent (m_renderer);
    SDL_Delay ((Uint32)(seconds * 1000));
}

void stopAll (bool &quit)
{
    quit = true;
}

void stopThisScript (bool &stopFlag)
{
    stopFlag = true;
}

void repeatBlock (int n, int &counter)
{
    counter = n;
}

bool repeatDone (int &counter)
{
    if (counter <= 0) return true;
    counter--;
    return false;
}

bool ifBlock (bool condition)
{
    return condition;
}

bool ifElseBlock (bool condition)
{
    return condition;
}

void foreverStart () {}

bool untilCondition (bool condition)
{
    return !condition;
}

#endif //CONTROL_BLOCK_H