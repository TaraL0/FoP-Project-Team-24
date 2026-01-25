
#ifndef SETUP_SCREEN_H
#define SETUP_SCREEN_H

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
//SDL_SetRenderTarget (m_renderer, bkImg);
// Clear the window with a background
SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
SDL_RenderClear (m_renderer);
SDL_SetRenderDrawColor (m_renderer, 255, 255, 255, 255);
SDL_Rect stage = {870, 120, 620, 480};
SDL_RenderFillRect (m_renderer, &stage);
SDL_Rect panel = {870, 615, 490, 130};
SDL_RenderFillRect (m_renderer, &panel);
SDL_Rect panel2 = {1370, 615, 120, 500};
SDL_RenderFillRect (m_renderer, &panel2);
SDL_Rect codeMenu = {10, 120, 100, 900};
SDL_RenderFillRect (m_renderer, &codeMenu);
SDL_Rect blocks = {112, 120, 330, 900};
SDL_RenderFillRect (m_renderer, &blocks);
SDL_Rect coding = {445, 120, 410, 900};
SDL_RenderFillRect (m_renderer, &coding);
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
SDL_SetRenderDrawBlendMode (m_renderer, SDL_BLENDMODE_BLEND);
SDL_SetRenderDrawColor (m_renderer, 0, 110, 250, 150);
SDL_Rect motionIcon = {10, 120, 100, 60};
SDL_RenderFillRect (m_renderer, &motionIcon);
SDL_SetRenderDrawColor (m_renderer, 100, 0, 200, 150);
SDL_Rect looksIcon = {10, 180, 100, 60};
SDL_RenderFillRect (m_renderer, &looksIcon);
SDL_SetRenderDrawColor (m_renderer, 200, 0, 150, 150);
SDL_Rect soundIcon = {10, 240, 100, 60};
SDL_RenderFillRect (m_renderer, &soundIcon);
TTF_Font *font = TTF_OpenFont ("arial.ttf", 24);
if (font)
{
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Blended (font, "Motion", textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
    SDL_Rect textRect = {24, 137, textSurface -> w, textSurface -> h};
    SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
    textSurface = TTF_RenderText_Blended (font, "Looks", textColor);
    textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
    textRect = {28, 197, textSurface -> w, textSurface -> h};
    SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
    textSurface = TTF_RenderText_Blended (font, "Sound", textColor);
    textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
    textRect = {24, 257, textSurface -> w, textSurface -> h};
    SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
    textColor = {0, 0, 0};
    textSurface = TTF_RenderText_Blended (font, "Code", textColor);
    textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
    textRect = {53, 75, textSurface -> w, textSurface -> h};
    SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
    textSurface = TTF_RenderText_Blended (font, "Costumes", textColor);
    textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
    textRect = {150, 75, textSurface -> w, textSurface -> h};
    SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
    textSurface = TTF_RenderText_Blended (font, "Sounds", textColor);
    textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
    textRect = {285, 75, textSurface -> w, textSurface -> h};
    SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture (textTexture);
    SDL_FreeSurface (textSurface);
}
else {cout<<TTF_GetError ();}

#endif //SETUP_SCREEN_H