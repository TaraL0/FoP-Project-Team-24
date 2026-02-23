
#ifndef SETUP_SCREEN_H
#define SETUP_SCREEN_H

#include "sprite.h"

void setupScreen (SDL_Renderer *m_renderer, TTF_Font *font, Stage &stage)
{
    SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
    SDL_RenderClear (m_renderer);
    SDL_SetRenderDrawColor (m_renderer, 255, 255, 255, 255);
    SDL_Rect panel = {870, 615, 490, 130};
    SDL_RenderFillRect (m_renderer, &panel);
    SDL_Rect mainStage = {870, 120, 620, 480};
    SDL_RenderFillRect (m_renderer, &mainStage);
    stage.draw (m_renderer, mainStage);
    SDL_Rect panel2 = {1370, 615, 120, 500};
    SDL_RenderFillRect (m_renderer, &panel2);
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
    SDL_SetRenderDrawColor (m_renderer, 128, 0, 128, 255);
    SDL_Rect addExtensionIcon = {10, 942, 100, 60};
    SDL_RenderFillRect (m_renderer, &addExtensionIcon);


    if (font)
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, "Code", textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_Rect textRect = {58, 75, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
        textSurface = TTF_RenderText_Blended (font, "Costumes", textColor);
        textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        textRect = {157, 75, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
        textSurface = TTF_RenderText_Blended (font, "Sounds", textColor);
        textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        textRect = {290, 75, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
        textSurface = TTF_RenderText_Blended (font, "Extension", textColor);
        textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        textRect = {13, 959, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
}

#endif //SETUP_SCREEN_H