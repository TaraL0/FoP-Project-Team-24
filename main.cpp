#include "setup_screen.h"
#include "motion_block.h"
#include "looks_block.h"
#include "sound_block.h"
#include "add_extension_pen.h"

int main (int argc, char *argv [])
{
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER ;
    Uint32 WND_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Init (SDL_flags);
    TTF_Init();
    SDL_CreateWindowAndRenderer (1920, 480, WND_flags, &m_window, &m_renderer);
    SDL_RaiseWindow (m_window);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode (0, &DM);
    Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Font *font = TTF_OpenFont ("arial.ttf", 24);
    TTF_Font *font2 = TTF_OpenFont ("arial.ttf", 18); // is used for thinking or saying blocks.
    Stage stage;
    setupScreen (m_renderer, font, stage);
    int numOfSprites = 0;

    vector <Sprite> sprites (100);
    vector <Sprite> spritesDisplayQueue;
    sprites [0].x =1180, sprites [0].y = 360;
    sprites [0].mainName = "Sprite1";
    sprites [0].costumes.push_back (IMG_LoadTexture (m_renderer, "scratch_cat.png"));
    sprites [0].costumeSetup ("scratch_cat");
    sprites [0].costumes.push_back (IMG_LoadTexture (m_renderer, "costume2.png"));
    sprites [0].costumeSetup ("costume2");
    sprites [0].m_sound = Mix_LoadWAV ("Meow.wav");
    sprites [0].layer = 0;
    spritesDisplayQueue.push_back (sprites [0]);

    sprites [1].x = 1190, sprites [1].y = 330;
    sprites [1].mainName = "Sprite2";
    sprites [1].costumes.push_back (IMG_LoadTexture (m_renderer, "ghost.png"));
    sprites [1].costumeSetup ("ghost");
    sprites [1].scale = 10;
    sprites [1].layer = 1;
    spritesDisplayQueue.push_back (sprites [1]);

    sprites [2].x = 1180, sprites [2].y = 390;
    sprites [2].mainName = "Sprite3";
    sprites [2].costumes.push_back (IMG_LoadTexture (m_renderer, "human.png"));
    sprites [2].costumeSetup ("human");
    sprites [2].scale = 10;
    sprites [2].layer = 2;
    spritesDisplayQueue.push_back (sprites [2]);
    numOfSprites = spritesDisplayQueue.size ();

    vector <SDL_Event> events;
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent (&event))
        {
            SDL_Rect button = {300, 300, 50, 50};
            SDL_SetRenderDrawColor (m_renderer, 0, 200, 0, 255);
            SDL_RenderFillRect (m_renderer, &button);
            for (auto &j : spritesDisplayQueue) {j.draw (m_renderer);}
            if (event.type == SDL_KEYDOWN) {quit = true;}
            events.push_back (event);
            if (event.type == SDL_MOUSEBUTTONDOWN and event.button.button == SDL_BUTTON_LEFT)
            {
                int xm, ym;
                SDL_GetMouseState (&xm, &ym);
                if (xm >= button.x and xm <= button.x + button.w and ym >= button.y and ym <= button.y + button.h)
                {
                    SDL_RenderClear (m_renderer);
                    setupScreen (m_renderer, font, stage);
                    SDL_RenderFillRect (m_renderer, &button);
                    for (auto &j : spritesDisplayQueue) {j.draw (m_renderer);}
                }
            }
            SDL_RenderPresent (m_renderer);
        }
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