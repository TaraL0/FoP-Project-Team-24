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
#include "backdrop_menu.h"

int main (int argc, char *argv [])
{
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
    Uint32 WND_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Init (SDL_flags);
    TTF_Init ();
    SDL_CreateWindowAndRenderer (1920, 480, WND_flags, &m_window, &m_renderer);
    SDL_RaiseWindow (m_window);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode (0, &DM);
    Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Font *font  = TTF_OpenFont ("arial.ttf", 24);
    TTF_Font *font2 = TTF_OpenFont ("arial.ttf", 18);
    Stage stage;
    bool activatePenIcon = false;

    BackdropLibrary backdropLib;
    backdropLib.load (m_renderer, "Blue Sky", "backdrop_sky.png");
    backdropLib.load (m_renderer, "Forest",   "backdrop_forest.png");
    if (!backdropLib.entries.empty ())
        backdropLib.setActive (0, stage);

    setupScreen (m_renderer, font, stage, activatePenIcon);

    VarManager varMgr;
    varMgr.defineVar ("score");
    varMgr.defineVar ("lives");
    varMgr.setVar ("lives", 3);

    MessageBus msgBus;
    Debugger dbg;
    Uint32 timerStart = SDL_GetTicks ();
    BlockRegistry blockReg;

    SDL_Rect makeBlockBtn    = {10, 720, 100, 40};
    SDL_Rect backdropMenuBtn = {10, 770, 100, 40};

    vector <Sprite> sprites (100);
    vector <Sprite> spritesDisplayQueue;

    sprites [0].x = 1180; sprites [0].y = 360;
    sprites [0].mainName = "Sprite1";
    sprites [0].costumes.push_back (IMG_LoadTexture (m_renderer, "scratch_cat.png"));
    sprites [0].costumeSetup ("scratch_cat");
    sprites [0].costumes.push_back (IMG_LoadTexture (m_renderer, "costume2.png"));
    sprites [0].costumeSetup ("costume2");
    sprites [0].m_sound = Mix_LoadWAV ("Meow.wav");
    sprites [0].layer = 0;
    spritesDisplayQueue.push_back (sprites [0]);

    sprites [1].x = 1190; sprites [1].y = 330;
    sprites [1].mainName = "Sprite2";
    sprites [1].costumes.push_back (IMG_LoadTexture (m_renderer, "ghost.png"));
    sprites [1].costumeSetup ("ghost");
    sprites [1].scale = 10;
    sprites [1].layer = 1;
    spritesDisplayQueue.push_back (sprites [1]);

    sprites [2].x = 1180; sprites [2].y = 390;
    sprites [2].mainName = "Sprite3";
    sprites [2].costumes.push_back (IMG_LoadTexture (m_renderer, "human.png"));
    sprites [2].costumeSetup ("human");
    sprites [2].scale = 10;
    sprites [2].layer = 2;
    spritesDisplayQueue.push_back (sprites [2]);

    int numOfSprites = spritesDisplayQueue.size ();

    bool drag0 = false;
    int offx0 = 0, offy0 = 0;
    SDL_Rect flagBtn = {870, 75, 40, 40};
    bool programRunning = false;

    vector <SDL_Event> events;
    bool quit = false;

    while (!quit)
    {
        SDL_Event event;
        events.clear ();
        while (SDL_PollEvent (&event))
        {
            events.push_back (event);
            if (event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_ESCAPE)
                quit = true;

            if (event.type == SDL_MOUSEBUTTONDOWN and event.button.button == SDL_BUTTON_LEFT)
            {
                int mx, my;
                SDL_GetMouseState (&mx, &my);
                auto hit = [&](SDL_Rect r){ return mx>=r.x and mx<=r.x+r.w and my>=r.y and my<=r.y+r.h; };

                if (hit (makeBlockBtn))
                {
                    CustomBlockDefResult res = runCustomBlockMenu (m_renderer, font, blockReg);
                    if (res.confirmed)
                        blockReg.define (res.blockName, res.params, [](vector <Param> &p) {});
                }

                if (hit (backdropMenuBtn))
                    runBackdropMenu (m_renderer, font, stage, backdropLib);

                if (hit (flagBtn))
                {
                    programRunning = true;
                    resetTimer (timerStart);
                }
            }
        }

        SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
        SDL_RenderClear (m_renderer);
        setupScreen (m_renderer, font, stage, activatePenIcon);

        SDL_SetRenderDrawColor (m_renderer, 0, 200, 0, 255);
        SDL_RenderFillRect (m_renderer, &flagBtn);

        SDL_SetRenderDrawColor (m_renderer, 255, 100, 0, 255);
        SDL_RenderFillRect (m_renderer, &makeBlockBtn);
        if (font2)
        {
            SDL_Color w = {255, 255, 255};
            SDL_Surface *s = TTF_RenderText_Blended (font2, "My Block", w);
            SDL_Texture *t = SDL_CreateTextureFromSurface (m_renderer, s);
            SDL_Rect r = {makeBlockBtn.x + 4, makeBlockBtn.y + 10, s -> w, s -> h};
            SDL_RenderCopy (m_renderer, t, nullptr, &r);
            SDL_DestroyTexture (t);
            SDL_FreeSurface (s);
        }

        SDL_SetRenderDrawColor (m_renderer, 60, 60, 180, 255);
        SDL_RenderFillRect (m_renderer, &backdropMenuBtn);
        if (font2)
        {
            SDL_Color w = {255, 255, 255};
            SDL_Surface *s = TTF_RenderText_Blended (font2, "Backdrops", w);
            SDL_Texture *t = SDL_CreateTextureFromSurface (m_renderer, s);
            SDL_Rect r = {backdropMenuBtn.x + 4, backdropMenuBtn.y + 10, s -> w, s -> h};
            SDL_RenderCopy (m_renderer, t, nullptr, &r);
            SDL_DestroyTexture (t);
            SDL_FreeSurface (s);
        }

        if (programRunning)
        {
            if (!events.empty ())
                startDrag (spritesDisplayQueue [0], events [0], drag0, offx0, offy0);

            if (touchingEdge (spritesDisplayQueue [0]))
                spritesDisplayQueue [0].direction = -spritesDisplayQueue [0].direction;

            if (touchingSprite (spritesDisplayQueue [0], spritesDisplayQueue [1]))
                varMgr.changeVar ("score", 1);

            if (keyPressed (SDLK_RIGHT))
                moveSteps (spritesDisplayQueue [0], 3);
            if (keyPressed (SDLK_LEFT))
            {
                turnLeft (spritesDisplayQueue [0], 5);
                moveSteps (spritesDisplayQueue [0], 3);
            }

            varMgr.drawAll (m_renderer, font2);

            if (font2)
            {
                string timerTxt = "timer: " + to_string ((int)timerValue (timerStart)) + "s";
                SDL_Color tc = {0, 100, 200};
                SDL_Surface *ts = TTF_RenderText_Blended (font2, timerTxt.c_str (), tc);
                SDL_Texture *tt = SDL_CreateTextureFromSurface (m_renderer, ts);
                SDL_Rect bg = {873, 308, ts -> w + 4, ts -> h + 4};
                SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
                SDL_RenderFillRect (m_renderer, &bg);
                SDL_Rect tr = {875, 310, ts -> w, ts -> h};
                SDL_RenderCopy (m_renderer, tt, nullptr, &tr);
                SDL_DestroyTexture (tt);
                SDL_FreeSurface (ts);
            }

            if (font2 and !blockReg.blocks.empty ())
            {
                string blist = "Blocks: ";
                for (int i = 0; i < (int)blockReg.blocks.size (); i++)
                {
                    blist += blockReg.blocks [i].name;
                    if (i < (int)blockReg.blocks.size () - 1) blist += ", ";
                }
                SDL_Color bc = {150, 0, 200};
                SDL_Surface *bs = TTF_RenderText_Blended (font2, blist.c_str (), bc);
                SDL_Texture *bt = SDL_CreateTextureFromSurface (m_renderer, bs);
                SDL_Rect bbg = {873, 338, bs -> w + 4, bs -> h + 4};
                SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
                SDL_RenderFillRect (m_renderer, &bbg);
                SDL_Rect br = {875, 340, bs -> w, bs -> h};
                SDL_RenderCopy (m_renderer, bt, nullptr, &br);
                SDL_DestroyTexture (bt);
                SDL_FreeSurface (bs);
            }
        }

        for (auto &j : spritesDisplayQueue) j.draw (m_renderer);
        dbg.drawAll (m_renderer);
        SDL_RenderPresent (m_renderer);
    }

    for (auto &j : sprites)
        for (auto &i : j.costumes)
            SDL_DestroyTexture (i);
    SDL_DestroyWindow (m_window);
    SDL_DestroyRenderer (m_renderer);
    IMG_Quit ();
    SDL_Quit ();
    TTF_Quit ();

    return 0;
}