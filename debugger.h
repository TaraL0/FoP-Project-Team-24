#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "sprite.h"

struct DebugLine
{
    int x1, y1, x2, y2;
    SDL_Color col;
};

struct Debugger
{
    bool active = false;
    vector <DebugLine> lines;
    SDL_Color runningColor = {0, 200, 255, 255};
    SDL_Color defaultColor = {255, 255, 0, 255};

    void pushLine (int x1, int y1, int x2, int y2)
    {
        if (!active) return;
        DebugLine dl;
        dl.x1 = x1; dl.y1 = y1;
        dl.x2 = x2; dl.y2 = y2;
        dl.col = runningColor;
        lines.push_back (dl);
    }

    void drawAll (SDL_Renderer *m_renderer)
    {
        if (!active) return;
        for (auto &dl : lines)
        {
            SDL_SetRenderDrawColor (m_renderer, dl.col.r, dl.col.g, dl.col.b, dl.col.a);
            SDL_RenderDrawLine (m_renderer, dl.x1, dl.y1, dl.x2, dl.y2);
        }
    }

    void markRunning (int idx)
    {
        for (int i = 0; i < (int)lines.size (); i++)
            lines [i].col = (i == idx) ? runningColor : defaultColor;
    }

    void clear ()
    {
        lines.clear ();
    }

    void toggle ()
    {
        active = !active;
        if (!active) clear ();
    }
};

void highlightRunningBlock (SDL_Renderer *m_renderer, SDL_Rect blockRect, bool isRunning)
{
    if (isRunning)
    {
        SDL_SetRenderDrawColor (m_renderer, 0, 200, 255, 180);
        SDL_Rect hl = {blockRect.x - 2, blockRect.y - 2, blockRect.w + 4, blockRect.h + 4};
        SDL_RenderDrawRect (m_renderer, &hl);
        SDL_RenderDrawRect (m_renderer, &blockRect);
    }
}

#endif //DEBUGGER_H