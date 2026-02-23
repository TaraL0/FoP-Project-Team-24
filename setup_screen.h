
#ifndef SETUP_SCREEN_H
#define SETUP_SCREEN_H

#include "sprite.h"

void setupScreen (SDL_Renderer *m_renderer, TTF_Font *font, Stage &stage)
{
    SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
    SDL_RenderClear (m_renderer);
    SDL_SetRenderDrawColor (m_renderer, 255, 255, 255, 255);
    SDL_Rect panel = {870, 615, 490, 130};
    SDL_RenderFillRect (m_renderer, &panel);
    SDL_Rect mainStage = {870, 120, 620, 480};
    SDL_RenderFillRect (m_renderer, &mainStage);
    stage.draw (m_renderer, mainStage);
    SDL_Rect panel2 = {1370, 615, 120, 500};
    SDL_RenderFillRect (m_renderer, &panel2);
    SDL_SetRenderDrawColor (m_renderer, 230, 230, 230, 255);
    SDL_Rect codeIcon = {30, 65, 105, 50};
    SDL_RenderFillRect (m_renderer, &codeIcon);
    SDL_Rect costumesIcon = {145, 65, 118, 50};
    SDL_RenderFillRect (m_renderer, &costumesIcon);
    SDL_Rect soundsIcon = {273, 65, 105, 50};
    SDL_RenderFillRect (m_renderer, &soundsIcon);
    SDL_Rect panel3 = {870, 745, 490, 350};
    SDL_RenderFillRect (m_renderer, &panel3);
    SDL_SetRenderDrawColor (m_renderer, 128, 0, 128, 255);
    SDL_Rect purpleMenu = {0, 0, 1600, 60};
    SDL_RenderFillRect (m_renderer, &purpleMenu);
    SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
    SDL_Rect extensionIcon = {500, 5, 100, 40};
    SDL_RenderFillRect (m_renderer, &extensionIcon);


    if (font)
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, "Code", textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_Rect textRect = {58, 75, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
        textSurface = TTF_RenderText_Blended (font, "Costumes", textColor);
        textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        textRect = {157, 75, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
        textSurface = TTF_RenderText_Blended (font, "Sounds", textColor);
        textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        textRect = {290, 75, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
        textSurface = TTF_RenderText_Blended (font, "Extension", textColor);
        textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        textRect = {503, 10, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
}

struct SpritePropPanel
{
    SDL_Rect panelRect = {870, 800, 490, 280};

    enum ActiveField { NONE, FIELD_NAME, FIELD_X, FIELD_Y, FIELD_DIR, FIELD_SIZE };
    ActiveField activeField = NONE;

    char nameBuf [64]  = {};
    char xBuf    [16]  = {};
    char yBuf    [16]  = {};
    char dirBuf  [16]  = {};
    char sizeBuf [16]  = {};

    bool initialized = false;

    void syncFromSprite (Sprite &sp)
    {
        int sx = sp.x - 1180;
        int sy = -(sp.y - 360);

        snprintf (nameBuf, sizeof nameBuf, "%s", sp.mainName.c_str ());
        snprintf (xBuf,    sizeof xBuf,    "%d", sx);
        snprintf (yBuf,    sizeof yBuf,    "%d", sy);
        snprintf (dirBuf,  sizeof dirBuf,  "%d", (int)(sp.direction + 90));
        snprintf (sizeBuf, sizeof sizeBuf, "%d", (int)sp.scale);
        initialized = true;
    }

    void applyToSprite (Sprite &sp)
    {
        sp.mainName = string (nameBuf);

        int sx = atoi (xBuf);
        int sy = atoi (yBuf);
        sp.oldx = sp.x;
        sp.oldy = sp.y;
        sp.x = sx + 1180;
        sp.y = -(sy) + 360;

        if (sp.x < 870)  sp.x = 870;
        if (sp.x > 1490) sp.x = 1490;
        if (sp.y < 120)  sp.y = 120;
        if (sp.y > 600)  sp.y = 600;

        int dir = atoi (dirBuf);
        sp.direction = dir - 90;

        int sz = atoi (sizeBuf);
        if (sz < 5)   sz = 5;
        if (sz > 300)  sz = 300;
        sp.scale = (double)sz;
    }

    void handleEvent (SDL_Event &ev, Sprite &sp)
    {
        if (!initialized) syncFromSprite (sp);

        if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT)
        {
            int mx = ev.button.x, my = ev.button.y;
            auto hit = [&](SDL_Rect r){ return mx>=r.x && mx<=r.x+r.w && my>=r.y && my<=r.y+r.h; };

            int lx = panelRect.x + 100;
            int fw = 140, fh = 28;
            int row0 = panelRect.y + 12;
            int row1 = panelRect.y + 52;
            int row2 = panelRect.y + 92;
            SDL_Rect rName = {lx, row0, 250, fh};
            SDL_Rect rX    = {lx, row1, fw, fh};
            SDL_Rect rY    = {lx + 180, row1, fw, fh};
            SDL_Rect rDir  = {lx, row2, fw, fh};
            SDL_Rect rSize = {lx + 180, row2, fw, fh};

            activeField = NONE;
            if (hit (rName)) activeField = FIELD_NAME;
            if (hit (rX))    activeField = FIELD_X;
            if (hit (rY))    activeField = FIELD_Y;
            if (hit (rDir))  activeField = FIELD_DIR;
            if (hit (rSize)) activeField = FIELD_SIZE;

            SDL_Rect btnShow = {panelRect.x + 100, panelRect.y + 132, 100, 34};
            SDL_Rect btnHide = {panelRect.x + 210, panelRect.y + 132, 100, 34};
            if (hit (btnShow)) { sp.visible = true; }
            if (hit (btnHide)) { sp.visible = false; }


            SDL_Rect btnApply = {panelRect.x + 100, panelRect.y + 178, 220, 38};
            if (hit (btnApply))
            {
                applyToSprite (sp);
                syncFromSprite (sp);
            }

            SDL_Rect btnReset = {panelRect.x + 330, panelRect.y + 178, 100, 38};
            if (hit (btnReset))
            {
                syncFromSprite (sp);
                activeField = NONE;
            }
        }

        if (ev.type == SDL_TEXTINPUT && activeField != NONE)
        {
            char *buf = getActiveBuf ();
            int maxLen = (activeField == FIELD_NAME) ? 60 : 12;
            int len = (int)strlen (buf);
            if (len < maxLen)
                strcat (buf, ev.text.text);
        }

        if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && activeField != NONE)
        {
            char *buf = getActiveBuf ();
            int len = (int)strlen (buf);
            if (len > 0) buf [len - 1] = '\0';
        }

        if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN && activeField != NONE)
        {
            applyToSprite (sp);
            syncFromSprite (sp);
            activeField = NONE;
        }
    }

    void render (SDL_Renderer *ren, TTF_Font *font, Sprite &sp)
    {
        if (!initialized) syncFromSprite (sp);

        SDL_SetRenderDrawColor (ren, 240, 240, 245, 255);
        SDL_RenderFillRect (ren, &panelRect);
        SDL_SetRenderDrawColor (ren, 180, 180, 190, 255);
        SDL_RenderDrawRect (ren, &panelRect);

        if (!font) return;

        SDL_Color black  = {0, 0, 0, 255};
        SDL_Color white  = {255, 255, 255, 255};
        SDL_Color accent = {80, 80, 200, 255};
        SDL_Color green  = {40, 160, 60, 255};
        SDL_Color red    = {200, 60, 60, 255};
        SDL_Color gray   = {120, 120, 120, 255};
        SDL_Color fieldBg    = {255, 255, 255, 255};
        SDL_Color fieldActBg = {235, 245, 255, 255};

        int lx  = panelRect.x + 100;
        int fw  = 140, fh = 28;
        int row0 = panelRect.y + 12;
        int row1 = panelRect.y + 52;
        int row2 = panelRect.y + 92;

        drawText (ren, font, "Sprite", accent, panelRect.x + 10, panelRect.y - 24);

        drawText (ren, font, "Name:", black, panelRect.x + 10, row0 + 2);
        drawField (ren, font, {lx, row0, 250, fh}, nameBuf, activeField == FIELD_NAME);

        drawText (ren, font, "x:", black, panelRect.x + 10, row1 + 2);
        drawField (ren, font, {lx, row1, fw, fh}, xBuf, activeField == FIELD_X);

        drawText (ren, font, "y:", black, lx + 155, row1 + 2);
        drawField (ren, font, {lx + 180, row1, fw, fh}, yBuf, activeField == FIELD_Y);

        drawText (ren, font, "Dir:", black, panelRect.x + 10, row2 + 2);
        drawField (ren, font, {lx, row2, fw, fh}, dirBuf, activeField == FIELD_DIR);

        drawText (ren, font, "Size:", black, lx + 148, row2 + 2);
        drawField (ren, font, {lx + 180, row2, fw, fh}, sizeBuf, activeField == FIELD_SIZE);

        SDL_Rect btnShow = {panelRect.x + 100, panelRect.y + 132, 100, 34};
        SDL_Rect btnHide = {panelRect.x + 210, panelRect.y + 132, 100, 34};

        drawText (ren, font, "Visible:", black, panelRect.x + 10, panelRect.y + 136);

        if (sp.visible)
        {
            drawButton (ren, font, btnShow, "Show", green, white);
            drawButton (ren, font, btnHide, "Hide", {190,190,190,255}, black);
        }
        else
        {
            drawButton (ren, font, btnShow, "Show", {190,190,190,255}, black);
            drawButton (ren, font, btnHide, "Hide", red, white);
        }

        int sx = sp.x - 1180;
        int sy = -(sp.y - 360);
        char live [128];
        snprintf (live, sizeof live, "Live:  x=%d  y=%d  dir=%d  size=%d  %s",
                  sx, sy, (int)(sp.direction + 90), (int)sp.scale,
                  sp.visible ? "shown" : "hidden");
        drawText (ren, font, live, gray, panelRect.x + 10, panelRect.y + 230);

        SDL_Rect btnApply = {panelRect.x + 100, panelRect.y + 178, 220, 38};
        SDL_Rect btnReset = {panelRect.x + 330, panelRect.y + 178, 100, 38};
        drawButton (ren, font, btnApply, "Apply Changes", accent, white);
        drawButton (ren, font, btnReset, "Reset", {160,160,160,255}, black);
    }

private:

    char *getActiveBuf ()
    {
        switch (activeField)
        {
            case FIELD_NAME: return nameBuf;
            case FIELD_X:    return xBuf;
            case FIELD_Y:    return yBuf;
            case FIELD_DIR:  return dirBuf;
            case FIELD_SIZE: return sizeBuf;
            default:         return nameBuf;
        }
    }

    void drawText (SDL_Renderer *ren, TTF_Font *font, const string &s,
                   SDL_Color c, int x, int y)
    {
        SDL_Surface *sf = TTF_RenderText_Blended (font, s.c_str (), c);
        if (!sf) return;
        SDL_Texture *tx = SDL_CreateTextureFromSurface (ren, sf);
        SDL_Rect r = {x, y, sf->w, sf->h};
        SDL_RenderCopy (ren, tx, nullptr, &r);
        SDL_DestroyTexture (tx);
        SDL_FreeSurface (sf);
    }

    void drawField (SDL_Renderer *ren, TTF_Font *font, SDL_Rect r,
                    const char *buf, bool active)
    {
        SDL_Color bg = active ? SDL_Color{235,245,255,255} : SDL_Color{255,255,255,255};
        SDL_SetRenderDrawColor (ren, bg.r, bg.g, bg.b, 255);
        SDL_RenderFillRect (ren, &r);

        SDL_Color border = active ? SDL_Color{80,80,200,255} : SDL_Color{160,160,160,255};
        SDL_SetRenderDrawColor (ren, border.r, border.g, border.b, 255);
        SDL_RenderDrawRect (ren, &r);

        string disp = string (buf);
        if (active) disp += "|";
        if (!disp.empty ())
            drawText (ren, font, disp, {0,0,0,255}, r.x + 4, r.y + 3);
    }

    void drawButton (SDL_Renderer *ren, TTF_Font *font, SDL_Rect r,
                     const string &label, SDL_Color bg, SDL_Color fg)
    {
        roundedBoxRGBA (ren, r.x, r.y, r.x + r.w, r.y + r.h,
                        4, bg.r, bg.g, bg.b, bg.a);
        SDL_SetRenderDrawColor (ren, 60, 60, 60, 255);
        SDL_RenderDrawRect (ren, &r);

        SDL_Surface *sf = TTF_RenderText_Blended (font, label.c_str (), fg);
        if (!sf) return;
        SDL_Texture *tx = SDL_CreateTextureFromSurface (ren, sf);
        SDL_Rect d = {r.x + (r.w - sf->w) / 2, r.y + (r.h - sf->h) / 2, sf->w, sf->h};
        SDL_RenderCopy (ren, tx, nullptr, &d);
        SDL_DestroyTexture (tx);
        SDL_FreeSurface (sf);
    }
};

#endif //SETUP_SCREEN_H