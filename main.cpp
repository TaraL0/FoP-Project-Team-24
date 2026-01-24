#include "motion_block.h"
#include "sound_block.h"

using namespace std;

int main (int argc, char *argv[])
{
    #include "setup_screen.h"

    Sprite sprite;
    sprite.m_img = IMG_LoadTexture (m_renderer, "scratch_cat.png");
    sprite.draw (m_renderer);

    SDL_Delay (3000);
    moveSteps (sprite, 10, m_renderer);
    SDL_Delay (1000);
    turnRight (sprite, 90, m_renderer);
    SDL_Delay (1000);
    turnLeft (sprite, 180, m_renderer);
    SDL_Delay (1000);
    goToCoords (sprite, 700, 300, m_renderer);
    SDL_Delay (1000);
    changeCoordsBy (sprite, 100, 100, m_renderer);
    SDL_Delay (1000);
    pointInDirection (sprite, -90, m_renderer);

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

    return 0;
}