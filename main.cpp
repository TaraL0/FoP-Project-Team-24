#include <windows.h>
#include <commdlg.h>
#include "tinyfiledialogs.h"
#include <SDL2/SDL_syswm.h>
#include "setup_screen.h"
#include "motion_block.h"
#include "looks_block.h"
#include "sound_block.h"
#include "add_extension_pen.h"
#include "control_block.h"
#include "sensing_block.h"
#include "operator_block.h"
#include "variable_block.h"
#include "myblock_block.h"
#include "event_block.h"
#include "debugger.h"
#include "costume_menu.h"
#include "coding_engine.h"
#include "backdrop_menu.h"

SDL_Rect flagBtn = {880, 78, 50, 36};
SDL_Rect stopBtn = {940, 78, 50, 36};

void drawFlag (SDL_Renderer *r, TTF_Font *f)
{
    roundedBoxRGBA (r, flagBtn.x, flagBtn.y,
                    flagBtn.x + flagBtn.w, flagBtn.y + flagBtn.h,
                    6, 40, 180, 60, 255);
    if (!f) return;
    SDL_Surface *s = TTF_RenderUTF8_Blended (f, "\xe2\x96\xb6", {255,255,255,255});
    if (!s) return;
    SDL_Texture *t = SDL_CreateTextureFromSurface (r, s);
    SDL_Rect d = {flagBtn.x + 14, flagBtn.y + 6, s->w, s->h};
    SDL_RenderCopy (r, t, nullptr, &d);
    SDL_DestroyTexture (t);
    SDL_FreeSurface (s);
}

void drawStop (SDL_Renderer *r, TTF_Font *f)
{
    roundedBoxRGBA (r, stopBtn.x, stopBtn.y,
                    stopBtn.x + stopBtn.w, stopBtn.y + stopBtn.h,
                    6, 220, 50, 50, 255);
    if (!f) return;
    SDL_Surface *s = TTF_RenderUTF8_Blended (f, "\xe2\x96\xa0", {255,255,255,255});
    if (!s) return;
    SDL_Texture *t = SDL_CreateTextureFromSurface (r, s);
    SDL_Rect d = {stopBtn.x + 16, stopBtn.y + 6, s->w, s->h};
    SDL_RenderCopy (r, t, nullptr, &d);
    SDL_DestroyTexture (t);
    SDL_FreeSurface (s);
}

bool inside (int mx, int my, SDL_Rect &r)
{
    return mx >= r.x && mx < r.x + r.w && my >= r.y && my < r.y + r.h;
}

int main (int, char *[])
{
    SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init ();
    IMG_Init (IMG_INIT_PNG);
    Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window   *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_CreateWindowAndRenderer (1920, 480, SDL_WINDOW_FULLSCREEN_DESKTOP,
                                &win, &ren);
    SDL_RaiseWindow (win);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode (0, &DM);

    TTF_Font *font  = TTF_OpenFont ("arial.ttf", 21);
    TTF_Font *font2 = TTF_OpenFont ("arial.ttf", 18);
    costumeEditor.init(font);
    Stage stage;
    SDL_Texture *penLayer = SDL_CreateTexture (ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DM.w, DM.h);
    SDL_SetTextureBlendMode (penLayer, SDL_BLENDMODE_BLEND);
    SDL_Texture *costumeMenuScreen = SDL_CreateTexture (ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DM.w, DM.h);
    SDL_SetTextureBlendMode (penLayer, SDL_BLENDMODE_BLEND);
    SDL_Texture *extensionMenuScreen = SDL_CreateTexture (ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DM.w, DM.h);
    SDL_SetTextureBlendMode (penLayer, SDL_BLENDMODE_BLEND);

    vector<Sprite> sprites (1);
    sprites[0].x = 1180;
    sprites[0].y = 360;
    sprites[0].mainName = "Sprite1";
    sprites[0].costumes.push_back (IMG_LoadTexture (ren, "scratch_cat.png"));
    sprites[0].costumeSetup ("scratch_cat");
    sprites[0].m_sound = Mix_LoadWAV ("Meow.wav");
    sprites[0].layer = 0;

    Pen pen0;
    VarManager  varMgr;
    MessageBus  msgBus;
    CodingEngine engine;
    engine.init ();


    engine.paletteRect = {0,   120, 220, DM.h - 120};
    engine.scriptRect  = {220, 120, 640, DM.h - 120};

    bool quit = false, activatePenIcon = false;

    while (!quit)
    {
        SDL_Event ev;
        while (SDL_PollEvent (&ev))
        {
            if (ev.type == SDL_QUIT)
                quit = true;

            if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
                quit = true;

            if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT)
            {
                int mx = ev.button.x, my = ev.button.y;
                if (inside (mx, my, flagBtn))  engine.startGreenFlag ();
                if (inside (mx, my, stopBtn))  engine.stopAll ();
            }

            engine.handleEvent (ev, font2);
            costumeEditor.handleEvent(ev, ren);
            setupCostumeMenuScreen(ren, costumeMenuScreen, ev, stage, font, sprites[0]);
            setupExtensionScreen(ren, extensionMenuScreen, ev, font, activatePenIcon);
        }

        if (engine.dragIdx < 0)
            engine.autoRebuildChains ();

        if (engine.globalRunning)
        {
            engine.updateAll (sprites[0], stage, sprites, pen0, penLayer, varMgr, msgBus, font2, ren, quit);
        }
        SDL_SetRenderDrawColor (ren, 255, 255, 255, 255);
        SDL_RenderClear (ren);
        setupScreen (ren, font, stage);
        SDL_RenderCopy (ren, penLayer, nullptr, nullptr);
        engine.render (ren, font2);
        drawFlag (ren, font2);
        drawStop (ren, font2);
        varMgr.drawAll (ren, font2);
        setupCostumeMenuScreen(ren, costumeMenuScreen, ev, stage, font, sprites[0]);
        for (auto &sp : sprites)
            sp.draw (ren);
        setupExtensionScreen(ren, extensionMenuScreen, ev, font, activatePenIcon);
        SDL_RenderPresent (ren);
        SDL_Delay (16);
    }

    for (auto &sp : sprites)
        for (auto &c : sp.costumes)
            if (c) SDL_DestroyTexture (c);

    SDL_DestroyTexture (penLayer);
    SDL_DestroyRenderer (ren);
    SDL_DestroyWindow (win);
    if (font)  TTF_CloseFont (font);
    if (font2) TTF_CloseFont (font2);
    Mix_CloseAudio ();
    IMG_Quit ();
    TTF_Quit ();
    SDL_Quit ();
    return 0;
}