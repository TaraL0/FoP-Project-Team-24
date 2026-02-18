
#include "setup_screen.h"
#include "motion_block.h"
#include "looks_block.h"
#include "sound_block.h"
#include "add_extension_pen.h"
#include "block_system.h"
#include "block_manager.h"

int main(int argc, char *argv[])
{
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
    Uint32 WND_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Init(SDL_flags);
    TTF_Init();
    SDL_CreateWindowAndRenderer(1920, 1080, WND_flags, &m_window, &m_renderer);
    SDL_RaiseWindow(m_window);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
    TTF_Font *font2 = TTF_OpenFont("arial.ttf", 18);
    TTF_Font *blockFont = TTF_OpenFont("arial.ttf", 14);  // فونت بلوک‌ها

    Stage stage;

    int numOfSprites = 0;
    vector<Sprite> sprites(100);
    vector<Sprite> spritesDisplayQueue;


    sprites[0].x = 1180;
    sprites[0].y = 360;
    sprites[0].mainName = "Sprite1";
    sprites[0].costumes.push_back(IMG_LoadTexture(m_renderer, "scratch_cat.png"));
    sprites[0].costumeSetup("scratch_cat");
    sprites[0].costumes.push_back(IMG_LoadTexture(m_renderer, "costume2.png"));
    sprites[0].costumeSetup("costume2");
    sprites[0].m_sound = Mix_LoadWAV("Meow.wav");
    sprites[0].layer = 0;
    spritesDisplayQueue.push_back(sprites[0]);

    sprites[1].x = 1190;
    sprites[1].y = 330;
    sprites[1].mainName = "Sprite2";
    sprites[1].costumes.push_back(IMG_LoadTexture(m_renderer, "ghost.png"));
    sprites[1].costumeSetup("ghost");
    sprites[1].scale = 10;
    sprites[1].layer = 1;
    spritesDisplayQueue.push_back(sprites[1]);

    sprites[2].x = 1180;
    sprites[2].y = 390;
    sprites[2].mainName = "Sprite3";
    sprites[2].costumes.push_back(IMG_LoadTexture(m_renderer, "human.png"));
    sprites[2].costumeSetup("human");
    sprites[2].scale = 10;
    sprites[2].layer = 2;
    spritesDisplayQueue.push_back(sprites[2]);

    numOfSprites = spritesDisplayQueue.size();

    BlockManager blockManager(m_renderer, blockFont);

    SDL_Rect greenFlagButton = {890, 75, 40, 40};
    SDL_Rect stopButton = {940, 75, 40, 40};

    bool quit = false;

    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                if (mouseX >= greenFlagButton.x && mouseX <= greenFlagButton.x + greenFlagButton.w &&
                    mouseY >= greenFlagButton.y && mouseY <= greenFlagButton.y + greenFlagButton.h)
                {
                    blockManager.executeAllScripts(sprites[0]);
                }
                else if (mouseX >= stopButton.x && mouseX <= stopButton.x + stopButton.w &&
                         mouseY >= stopButton.y && mouseY <= stopButton.y + stopButton.h)
                {
                    blockManager.stopExecution();
                }
                else
                {
                    blockManager.handleMouseDown(mouseX, mouseY);
                }
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                blockManager.handleMouseMove(mouseX, mouseY);
            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
            {
                blockManager.handleMouseUp(mouseX, mouseY);
            }
        }

        SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
        SDL_RenderClear(m_renderer);

        setupScreen(m_renderer, font, stage);


        SDL_SetRenderDrawColor(m_renderer, 0, 200, 0, 255);
        SDL_RenderFillRect(m_renderer, &greenFlagButton);

        filledTrigonRGBA(m_renderer,
                         greenFlagButton.x + 10, greenFlagButton.y + 8,
                         greenFlagButton.x + 10, greenFlagButton.y + 32,
                         greenFlagButton.x + 32, greenFlagButton.y + 20,
                         255, 255, 255, 255);


        SDL_SetRenderDrawColor(m_renderer, 200, 0, 0, 255);
        SDL_RenderFillRect(m_renderer, &stopButton);

        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_Rect stopIcon = {stopButton.x + 10, stopButton.y + 10, 20, 20};
        SDL_RenderFillRect(m_renderer, &stopIcon);


        blockManager.drawAll();


        for (auto &sprite : spritesDisplayQueue)
        {
            sprite.draw(m_renderer);
        }


        SDL_RenderPresent(m_renderer);

        SDL_Delay(16);  // تقریباً 60 FPS
    }

    for (auto &j : sprites)
    {
        for (auto &i : j.costumes)
        {
            SDL_DestroyTexture(i);
        }
        if (j.m_sound)
        {
            Mix_FreeChunk(j.m_sound);
        }
    }

    TTF_CloseFont(font);
    TTF_CloseFont(font2);
    TTF_CloseFont(blockFont);

    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);

    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
