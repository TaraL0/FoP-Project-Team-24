
#ifndef COSTUME_MENU_H
#define COSTUME_MENU_H

void uploadImage (SDL_Renderer *m_renderer, Sprite &sprite)
{
    const char *path = tinyfd_openFileDialog ("Select image", "", 0, nullptr, nullptr, 0);
    if (!path) return;
    string strPath = path;
    size_t pos = strPath.find_last_of ("\\/");
    size_t pos2 = strPath.find_last_of (".");
    string name = strPath.substr (pos + 1, pos2 - pos - 1);
    SDL_Texture *img = IMG_LoadTexture (m_renderer, path);
    if (!img) return;
    sprite.costumes.push_back (img);
    sprite.costumeSetup (name);
}

inline void setupCostumeMenuScreen (SDL_Renderer *m_renderer, SDL_Texture *costumeMenuScreen, SDL_Event &event, Stage &stage, TTF_Font *font, Sprite &sprite)
{
    static bool show = false;
    SDL_Rect costumesIcon = {145, 65, 118, 50};
    SDL_Rect codeIcon = {30, 65, 105, 50};
    if (event.type == SDL_MOUSEBUTTONDOWN and event.button.button == SDL_BUTTON_LEFT)
    {
        int xm, ym;
        SDL_GetMouseState (&xm, &ym);
        if (!show and xm >= costumesIcon.x and xm <= costumesIcon.x + costumesIcon.w
            and ym >= costumesIcon.y and ym <= costumesIcon.y + costumesIcon.h)
        {show = true;}
    }
    if (!show) return;
    SDL_SetRenderTarget (m_renderer, costumeMenuScreen);
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
    SDL_Rect editingPanel = {145, 120, 710, 900};
    SDL_RenderFillRect (m_renderer, &editingPanel);
    SDL_SetRenderDrawColor (m_renderer, 230, 230, 230, 255);
    SDL_RenderFillRect (m_renderer, &codeIcon);
    SDL_RenderFillRect (m_renderer, &costumesIcon);
    SDL_Rect soundsIcon = {273, 65, 105, 50};
    SDL_RenderFillRect (m_renderer, &soundsIcon);
    SDL_Rect panel3 = {870, 745, 490, 350};
    SDL_RenderFillRect (m_renderer, &panel3);
    SDL_SetRenderDrawColor (m_renderer, 128, 0, 128, 255);
    SDL_Rect purpleMenu = {0, 0, 1600, 60};
    SDL_RenderFillRect (m_renderer, &purpleMenu);
    SDL_SetRenderDrawColor (m_renderer, 128, 0, 128, 255);
    SDL_Rect uploadIcon = {22, 930, 100, 60};
    SDL_RenderFillRect (m_renderer, &uploadIcon);
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
        textSurface = TTF_RenderText_Blended (font, "Upload", textColor);
        textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        textRect = {38, 947, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
    if (event.type == SDL_MOUSEBUTTONDOWN and event.button.button == SDL_BUTTON_LEFT)
    {
        int xm, ym;
        SDL_GetMouseState (&xm, &ym);
        if (xm >= codeIcon.x and xm <= codeIcon.x + codeIcon.w and ym >= codeIcon.y and ym <= codeIcon.y + codeIcon.h)
        {
            show = false;
        }
        else if (xm >= uploadIcon.x and xm <= uploadIcon.x + uploadIcon.w and ym >= uploadIcon.y and ym <= uploadIcon.y + uploadIcon.h)
        {
            uploadImage (m_renderer, sprite);
        }
    }
    SDL_SetRenderTarget (m_renderer, nullptr);
    SDL_RenderCopy (m_renderer, costumeMenuScreen, nullptr, nullptr);
}

#endif //COSTUME_MENU_H