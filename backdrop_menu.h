#ifndef BACKDROP_MENU_H
#define BACKDROP_MENU_H

#include "sprite.h"

struct BackdropEntry
{
    string name;
    string path;
    SDL_Texture *tex = nullptr;
};

struct BackdropLibrary
{
    vector <BackdropEntry> entries;
    int active = 0;

    void load (SDL_Renderer *m_renderer, string name, string path)
    {
        BackdropEntry e;
        e.name = name;
        e.path = path;
        e.tex  = IMG_LoadTexture (m_renderer, path.c_str ());
        entries.push_back (e);
    }

    void setActive (int idx, Stage &stage)
    {
        if (idx < 0 or idx >= (int)entries.size ()) return;
        active = idx;
        if (idx < (int)stage.backdrops.size ())
            stage.curBackdropNum = idx;
        else
        {
            stage.backdrops.push_back (entries [idx].tex);
            stage.backdropsName.push_back (entries [idx].name);
            stage.curBackdropNum = (int)stage.backdrops.size () - 1;
        }
    }

    string activeName ()
    {
        if (entries.empty ()) return "(none)";
        return entries [active].name;
    }
};

struct BackdropEditor
{
    SDL_Texture *target = nullptr;
    string targetName;
    int brushSize = 8;
    SDL_Color brushColor = {0, 0, 0, 255};
    bool erasing = false;

    void draw (SDL_Renderer *m_renderer, SDL_Event &ev, SDL_Rect canvas)
    {
        if (!target) return;
        if (ev.type == SDL_MOUSEBUTTONDOWN or ev.type == SDL_MOUSEMOTION)
        {
            if (!(SDL_GetMouseState (nullptr, nullptr) & SDL_BUTTON (SDL_BUTTON_LEFT))) return;
            int mx, my;
            SDL_GetMouseState (&mx, &my);
            if (mx < canvas.x or mx > canvas.x + canvas.w or
                my < canvas.y or my > canvas.y + canvas.h) return;
            SDL_SetRenderTarget (m_renderer, target);
            if (erasing)
                SDL_SetRenderDrawColor (m_renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor (m_renderer, brushColor.r, brushColor.g, brushColor.b, 255);
            int lx = mx - canvas.x, ly = my - canvas.y;
            SDL_Rect dot = {lx - brushSize / 2, ly - brushSize / 2, brushSize, brushSize};
            SDL_RenderFillRect (m_renderer, &dot);
            SDL_SetRenderTarget (m_renderer, nullptr);
        }
    }
};

inline void runBackdropMenu (SDL_Renderer *m_renderer, TTF_Font *font, Stage &stage, BackdropLibrary &lib)
{
    enum SubMenu { MAIN, LIBRARY, LOAD_FILE, EDITOR } sub = MAIN;

    int pw = 900, ph = 620, px = (1920 - pw) / 2, py = (1080 - ph) / 2;
    SDL_Rect panel = {px, py, pw, ph};

    BackdropEditor editor;
    SDL_Texture *editTex = nullptr;
    int editIdx = -1;

    SDL_Color brushPalette [8] = {
        {0,0,0,255}, {255,255,255,255}, {255,0,0,255}, {0,200,0,255},
        {0,0,255,255}, {255,200,0,255}, {200,0,200,255}, {0,200,200,255}
    };

    char filePathBuf [512] = {};
    bool fileInputActive = false;
    string fileError;

    bool done = false;
    SDL_StartTextInput ();

    while (!done)
    {
        SDL_Event ev;
        while (SDL_PollEvent (&ev))
        {
            if (ev.type == SDL_KEYDOWN and ev.key.keysym.sym == SDLK_ESCAPE)
            { if (sub == MAIN) done = true; else sub = MAIN; }

            if (sub == LOAD_FILE and fileInputActive and ev.type == SDL_TEXTINPUT)
            {
                int len = strlen (filePathBuf);
                if (len < 510) strcat (filePathBuf, ev.text.text);
            }
            if (sub == LOAD_FILE and ev.type == SDL_KEYDOWN)
            {
                if (ev.key.keysym.sym == SDLK_BACKSPACE)
                {
                    int len = strlen (filePathBuf);
                    if (len > 0) filePathBuf [len - 1] = 0;
                }
            }

            if (sub == EDITOR and editTex)
                editor.draw (m_renderer, ev, {px + 20, py + 80, pw - 180, ph - 140});

            if (ev.type == SDL_MOUSEBUTTONDOWN and ev.button.button == SDL_BUTTON_LEFT)
            {
                int mx, my;
                SDL_GetMouseState (&mx, &my);
                auto hit = [&](SDL_Rect r){ return mx>=r.x and mx<=r.x+r.w and my>=r.y and my<=r.y+r.h; };

                if (sub == MAIN)
                {
                    SDL_Rect btnLib    = {px+20,  py+120, pw-40, 50};
                    SDL_Rect btnLoad   = {px+20,  py+190, pw-40, 50};
                    SDL_Rect btnClose  = {px+20,  py+ph-70, 120, 44};
                    if (hit (btnLib))   sub = LIBRARY;
                    if (hit (btnLoad))  { sub = LOAD_FILE; memset (filePathBuf,0,512); fileInputActive=true; fileError.clear(); }
                    if (hit (btnClose)) done = true;
                }
                else if (sub == LIBRARY)
                {
                    SDL_Rect btnBack = {px+20, py+ph-70, 120, 44};
                    if (hit (btnBack)) sub = MAIN;
                    for (int i = 0; i < (int)lib.entries.size (); i++)
                    {
                        SDL_Rect row = {px+20, py+80 + i*70, pw-40, 60};
                        SDL_Rect setBtn  = {px+pw-200, py+86 + i*70, 80, 46};
                        SDL_Rect editBtn = {px+pw-310, py+86 + i*70, 100, 46};
                        if (hit (setBtn))
                        {
                            lib.setActive (i, stage);
                        }
                        if (hit (editBtn))
                        {
                            editIdx = i;
                            int w, h;
                            SDL_QueryTexture (lib.entries [i].tex, nullptr, nullptr, &w, &h);
                            if (editTex) SDL_DestroyTexture (editTex);
                            editTex = SDL_CreateTexture (m_renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, w, h);
                            SDL_SetRenderTarget (m_renderer, editTex);
                            SDL_RenderCopy (m_renderer, lib.entries [i].tex, nullptr, nullptr);
                            SDL_SetRenderTarget (m_renderer, nullptr);
                            editor.target = editTex;
                            editor.targetName = lib.entries [i].name;
                            sub = EDITOR;
                        }
                    }
                }
                else if (sub == LOAD_FILE)
                {
                    SDL_Rect fieldRect = {px+20, py+160, pw-40, 44};
                    SDL_Rect loadBtn   = {px+20, py+220, 140, 44};
                    SDL_Rect btnBack   = {px+180,py+220, 120, 44};
                    if (hit (fieldRect)) fileInputActive = true;
                    if (hit (loadBtn))
                    {
                        string path = string (filePathBuf);
                        SDL_Texture *t = IMG_LoadTexture (m_renderer, path.c_str ());
                        if (!t)
                        {
                            fileError = "Could not load: " + path;
                        }
                        else
                        {
                            string nm = path;
                            size_t sl = nm.find_last_of ("/\\");
                            if (sl != string::npos) nm = nm.substr (sl + 1);
                            size_t dot = nm.find_last_of ('.');
                            if (dot != string::npos) nm = nm.substr (0, dot);
                            BackdropEntry e;
                            e.name = nm; e.path = path; e.tex = t;
                            lib.entries.push_back (e);
                            lib.setActive ((int)lib.entries.size () - 1, stage);
                            fileError.clear ();
                            sub = LIBRARY;
                        }
                    }
                    if (hit (btnBack)) sub = MAIN;
                }
                else if (sub == EDITOR)
                {
                    SDL_Rect btnBack    = {px+20,   py+ph-70, 120, 44};
                    SDL_Rect btnSave    = {px+160,  py+ph-70, 120, 44};
                    SDL_Rect btnErase   = {px+pw-155,py+80,  130, 36};
                    SDL_Rect btnDraw    = {px+pw-155,py+125, 130, 36};
                    SDL_Rect brushSmall = {px+pw-155,py+175,  55, 36};
                    SDL_Rect brushBig   = {px+pw-90, py+175,  55, 36};

                    if (hit (btnBack))  sub = LIBRARY;
                    if (hit (btnErase)) editor.erasing = true;
                    if (hit (btnDraw))  editor.erasing = false;
                    if (hit (brushSmall)) editor.brushSize = 4;
                    if (hit (brushBig))   editor.brushSize = 16;

                    if (hit (btnSave) and editIdx >= 0)
                    {
                        if (editTex)
                        {
                            lib.entries [editIdx].tex = editTex;
                            if (editIdx < (int)stage.backdrops.size ())
                                stage.backdrops [editIdx] = editTex;
                            editTex = nullptr;
                            editor.target = nullptr;
                        }
                        sub = LIBRARY;
                    }

                    for (int i = 0; i < 8; i++)
                    {
                        SDL_Rect swatch = {px+pw-155 + (i%4)*32, py+225 + (i/4)*32, 28, 28};
                        if (hit (swatch) and !editor.erasing)
                            editor.brushColor = brushPalette [i];
                    }
                }
            }
        }

        SDL_SetRenderDrawBlendMode (m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor (m_renderer, 0, 0, 0, 140);
        SDL_Rect overlay = {0, 0, 1920, 1080};
        SDL_RenderFillRect (m_renderer, &overlay);

        SDL_SetRenderDrawColor (m_renderer, 245, 245, 245, 255);
        SDL_RenderFillRect (m_renderer, &panel);
        SDL_SetRenderDrawColor (m_renderer, 80, 80, 80, 255);
        SDL_RenderDrawRect (m_renderer, &panel);

        if (!font) { SDL_RenderPresent (m_renderer); continue; }

        SDL_Color black  = {0,0,0,255};
        SDL_Color white  = {255,255,255,255};
        SDL_Color accent = {80,80,200,255};
        SDL_Color green  = {40,160,60,255};
        SDL_Color red    = {180,40,40,255};
        SDL_Color gray   = {160,160,160,255};
        SDL_Color orange = {220,110,0,255};

        auto txt = [&](string s, SDL_Color c, int x, int y)
        {
            SDL_Surface *sf = TTF_RenderText_Blended (font, s.c_str (), c);
            if (!sf) return;
            SDL_Texture *tx = SDL_CreateTextureFromSurface (m_renderer, sf);
            SDL_Rect r = {x, y, sf->w, sf->h};
            SDL_RenderCopy (m_renderer, tx, nullptr, &r);
            SDL_DestroyTexture (tx);
            SDL_FreeSurface (sf);
        };

        auto btn = [&](SDL_Rect r, string label, SDL_Color bg, SDL_Color fg)
        {
            SDL_SetRenderDrawColor (m_renderer, bg.r, bg.g, bg.b, bg.a);
            SDL_RenderFillRect (m_renderer, &r);
            SDL_SetRenderDrawColor (m_renderer, 40,40,40,255);
            SDL_RenderDrawRect (m_renderer, &r);
            txt (label, fg, r.x+8, r.y+10);
        };

        if (sub == MAIN)
        {
            txt ("Backdrop Settings", accent, px+20, py+20);
            txt ("Active: " + lib.activeName (), orange, px+20, py+65);

            SDL_Rect btnLib  = {px+20, py+120, pw-40, 50};
            SDL_Rect btnLoad = {px+20, py+190, pw-40, 50};
            btn (btnLib,  "Backdrop Library  (built-in backdrops)", {210,210,230,255}, black);
            btn (btnLoad, "Load Image from File", {210,230,210,255}, black);
            btn ({px+20, py+ph-70, 120, 44}, "Close", {180,60,60,255}, white);
        }
        else if (sub == LIBRARY)
        {
            txt ("Backdrop Library", accent, px+20, py+20);
            txt ("Active: " + lib.activeName (), orange, px+20, py+50);

            if (lib.entries.empty ())
                txt ("No backdrops loaded.", gray, px+20, py+90);

            for (int i = 0; i < (int)lib.entries.size (); i++)
            {
                SDL_Rect row = {px+20, py+80+i*70, pw-40, 60};
                SDL_Color rowBg = (i == lib.active) ? SDL_Color{200,230,255,255} : SDL_Color{225,225,225,255};
                SDL_SetRenderDrawColor (m_renderer, rowBg.r,rowBg.g,rowBg.b,255);
                SDL_RenderFillRect (m_renderer, &row);
                SDL_SetRenderDrawColor (m_renderer, 150,150,150,255);
                SDL_RenderDrawRect (m_renderer, &row);

                if (lib.entries [i].tex)
                {
                    SDL_Rect thumb = {px+25, py+85+i*70, 80, 48};
                    SDL_RenderCopy (m_renderer, lib.entries [i].tex, nullptr, &thumb);
                }

                string lbl = lib.entries [i].name;
                if (i == lib.active) lbl += "  [active]";
                txt (lbl, black, px+115, py+97+i*70);

                btn ({px+pw-310, py+86+i*70, 100, 46}, "Edit",    {100,160,220,255}, white);
                btn ({px+pw-200, py+86+i*70, 80,  46}, "Set",     green, white);
            }

            btn ({px+20, py+ph-70, 120, 44}, "Back", {120,120,120,255}, white);
        }
        else if (sub == LOAD_FILE)
        {
            txt ("Load Image from File", accent, px+20, py+20);
            txt ("Enter full file path (.png / .jpg):", black, px+20, py+120);

            SDL_Rect field = {px+20, py+160, pw-40, 44};
            SDL_SetRenderDrawColor (m_renderer, 255,255,255,255);
            SDL_RenderFillRect (m_renderer, &field);
            SDL_SetRenderDrawColor (m_renderer, fileInputActive?80:150, fileInputActive?80:150, fileInputActive?200:150, 255);
            SDL_RenderDrawRect (m_renderer, &field);
            string disp = string(filePathBuf) + (fileInputActive?"|":"");
            if (!disp.empty ()) txt (disp, black, field.x+6, field.y+10);

            btn ({px+20,  py+220, 140, 44}, "Load",   green, white);
            btn ({px+180, py+220, 120, 44}, "Back", {120,120,120,255}, white);

            if (!fileError.empty ())
                txt (fileError, red, px+20, py+280);
        }
        else if (sub == EDITOR)
        {
            txt ("Editor: " + editor.targetName, accent, px+20, py+20);

            SDL_Rect canvas = {px+20, py+60, pw-180, ph-120};
            SDL_SetRenderDrawColor (m_renderer, 255,255,255,255);
            SDL_RenderFillRect (m_renderer, &canvas);
            if (editTex)
            {
                int tw, th;
                SDL_QueryTexture (editTex, nullptr, nullptr, &tw, &th);
                double ar = (double)tw / th;
                int dw = canvas.w, dh = (int)(dw / ar);
                if (dh > canvas.h) { dh = canvas.h; dw = (int)(dh * ar); }
                SDL_Rect dst = {canvas.x + (canvas.w - dw)/2, canvas.y + (canvas.h - dh)/2, dw, dh};
                SDL_RenderCopy (m_renderer, editTex, nullptr, &dst);
            }
            SDL_SetRenderDrawColor (m_renderer, 80,80,80,255);
            SDL_RenderDrawRect (m_renderer, &canvas);

            int rx = px+pw-155;
            btn ({rx, py+80,  130, 36}, "Eraser",  editor.erasing?SDL_Color{220,100,100,255}:SDL_Color{210,210,210,255}, black);
            btn ({rx, py+125, 130, 36}, "Draw",   !editor.erasing?SDL_Color{100,180,220,255}:SDL_Color{210,210,210,255}, black);
            txt ("Size:", black, rx, py+168);
            btn ({rx,    py+185, 55, 36}, "S", {210,210,210,255}, black);
            btn ({rx+65, py+185, 55, 36}, "L", {210,210,210,255}, black);

            txt ("Color:", black, rx, py+230);
            for (int i = 0; i < 8; i++)
            {
                SDL_Rect sw = {rx+(i%4)*32, py+255+(i/4)*32, 28, 28};
                SDL_Color &c = brushPalette [i];
                SDL_SetRenderDrawColor (m_renderer, c.r,c.g,c.b,255);
                SDL_RenderFillRect (m_renderer, &sw);
                if (!editor.erasing and editor.brushColor.r==c.r and editor.brushColor.g==c.g and editor.brushColor.b==c.b)
                {
                    SDL_SetRenderDrawColor (m_renderer, 255,255,0,255);
                    SDL_RenderDrawRect (m_renderer, &sw);
                }
            }

            txt ("Current color:", black, rx, py+325);
            SDL_Rect curCol = {rx, py+350, 120, 30};
            SDL_SetRenderDrawColor (m_renderer, editor.brushColor.r,editor.brushColor.g,editor.brushColor.b,255);
            SDL_RenderFillRect (m_renderer, &curCol);

            btn ({px+20,  py+ph-70, 120, 44}, "Back",       {120,120,120,255}, white);
            btn ({px+160, py+ph-70, 120, 44}, "Save",       green,             white);
        }

        SDL_RenderPresent (m_renderer);
    }

    SDL_StopTextInput ();
    if (editTex) { SDL_DestroyTexture (editTex); }
}

#endif //BACKDROP_MENU_H