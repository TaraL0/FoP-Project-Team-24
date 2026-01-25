#include "motion_block.h"
#include "sound_block.h"
#include "looks_block.h"

using namespace std;

int main (int argc, char *argv[])
{
    #include "setup_screen.h"
    Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Sprite sprite;
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
            if (event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_ESCAPE) {quit = true;}
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