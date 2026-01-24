
#ifndef SETUP_SCREEN_H
#define SETUP_SCREEN_H

//Initialization of SDL windows
Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER ;
Uint32 WND_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;
SDL_Window *m_window;
SDL_Renderer *m_renderer;
SDL_Texture *bkImg = nullptr;

SDL_Init (SDL_flags);
SDL_CreateWindowAndRenderer (1920, 480, WND_flags, &m_window, &m_renderer);
//Pass the focus to the drawing window
SDL_RaiseWindow (m_window);
//Get screen resolution
SDL_DisplayMode DM;
SDL_GetCurrentDisplayMode (0, &DM);
int W = DM.w;
int H = DM.h;
bkImg = SDL_CreateTexture (m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, W, H);
//SDL_SetRenderTarget (m_renderer, bkImg);
// Clear the window with a white background
SDL_SetRenderDrawColor (m_renderer, 255, 255, 255, 255);
SDL_RenderClear (m_renderer);

// Show the window
SDL_RenderPresent (m_renderer);

Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 2048);

#endif //SETUP_SCREEN_H