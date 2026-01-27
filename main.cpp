#include "setup_screen.h"
#include "motion_block.h"
#include "looks_block.h"
#include "sound_block.h"

int main (int argc, char *argv [])
{
    //Initialization of SDL windows
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER ;
    Uint32 WND_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Texture *bkImg = nullptr;
    SDL_Init (SDL_flags);
    TTF_Init();
    SDL_CreateWindowAndRenderer (1920, 480, WND_flags, &m_window, &m_renderer);
    //Pass the focus to the drawing window
    SDL_RaiseWindow (m_window);
    //Get screen resolution
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode (0, &DM);
    int W = DM.w;
    int H = DM.h;
    bkImg = SDL_CreateTexture (m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, W, H);
    Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Font *font = TTF_OpenFont ("arial.ttf", 24);
    setupScreen (m_renderer, font);

    Sprite sprite;
    sprite.x =1180, sprite.y = 360;
    sprite.m_img = IMG_LoadTexture (m_renderer, "scratch_cat.png");
    sprite.draw (m_renderer);
    sprite.m_sound = Mix_LoadWAV ("Meow.wav");

    // Show the window
    SDL_RenderPresent (m_renderer);

    vector <SDL_Event> events;
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_KEYDOWN) {quit = true;}
            events.push_back (event);
        }
    }

    //Finalize and free resources
    SDL_DestroyTexture (sprite.m_img);
    SDL_DestroyWindow (m_window);
    SDL_DestroyRenderer (m_renderer);
    IMG_Quit ();
    SDL_Quit ();
    TTF_Quit ();

    return 0;
}