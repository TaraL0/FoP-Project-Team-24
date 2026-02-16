
#ifndef ADD_EXTENSION_PEN_H
#define ADD_EXTENSION_PEN_H

struct Pen
{
    bool penDown = true;
    SDL_Color penColor = {0, 0, 255, 255};
    int penWidth = 1;

    void draw (SDL_Renderer *m_renderer, Sprite &sprite, SDL_Texture *penLayer)
    {
        if (!penDown) return;
        SDL_SetRenderTarget (m_renderer, penLayer);
        thickLineRGBA (m_renderer, sprite.oldx, sprite.oldy, sprite.x, sprite.y,
            penWidth, penColor.r, penColor.g, penColor.b, penColor.a);
        SDL_SetRenderTarget (m_renderer, nullptr);
    }

    void penUp () {penDown = false;}

    void eraseAll (SDL_Renderer *m_renderer, SDL_Texture *penLayer)
    {
        if (!penLayer) return;
        SDL_SetRenderTarget (m_renderer, penLayer);
        SDL_SetRenderDrawColor (m_renderer, 0, 0, 0, 0);
        SDL_RenderClear (m_renderer);
        SDL_SetRenderTarget (m_renderer, nullptr);
    }

    void setPenColor (SDL_Color color) {penColor = color;}

    void setPenWidth (int width) {penWidth = width;}

    void changePenWidth (int width) {penWidth += width;}
};

void stamp (SDL_Renderer *m_renderer, Sprite &sprite, SDL_Texture *penLayer)
{
    vector <Sprite> stamps;
    stamps.push_back (sprite);
    SDL_SetRenderTarget (m_renderer, penLayer);
    for (auto &i : stamps)
    {
        i.draw (m_renderer);
    }
    SDL_SetRenderTarget (m_renderer, nullptr);
}

inline void setupExtensionScreen (SDL_Renderer *m_renderer, SDL_Texture *extensionMenuScreen, SDL_Event &event, TTF_Font *font, bool &activatePenIcon)
{
    static bool show = false;
    SDL_Rect addExtensionIcon = {10, 942, 100, 60};
    if (event.type == SDL_MOUSEBUTTONDOWN and event.button.button == SDL_BUTTON_LEFT)
    {
        int xm, ym;
        SDL_GetMouseState (&xm, &ym);
        if (!show and xm >= addExtensionIcon.x and xm <= addExtensionIcon.x + addExtensionIcon.w
            and ym >= addExtensionIcon.y and ym <= addExtensionIcon.y + addExtensionIcon.h)
        {show = true;}
    }
    if (!show) return;
    SDL_SetRenderTarget (m_renderer, extensionMenuScreen);
    SDL_SetRenderDrawColor (m_renderer, 255, 255, 255, 255);
    SDL_RenderClear (m_renderer);
    SDL_SetRenderDrawColor (m_renderer, 128, 0, 128, 255);
    SDL_Rect purpleMenu = {0, 0, 1600, 60};
    SDL_RenderFillRect (m_renderer, &purpleMenu);
    SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
    SDL_Rect goBackIcon = {100, 5, 80, 50};
    SDL_RenderFillRect (m_renderer, &goBackIcon);
    if (font)
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, "back", textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_Rect textRect = {115, 15, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
    SDL_Texture *penChoosingOption = IMG_LoadTexture (m_renderer, "pen option.png");
    int w, h;
    SDL_QueryTexture (penChoosingOption, nullptr, nullptr, &w, &h);
    SDL_Rect img_rect_penChoosingOption = {20, 80, w, h};
    SDL_RenderCopy (m_renderer, penChoosingOption, nullptr, &img_rect_penChoosingOption);
    if (event.type == SDL_MOUSEBUTTONDOWN and event.button.button == SDL_BUTTON_LEFT)
    {
        int xm, ym;
        SDL_GetMouseState (&xm, &ym);
        if (xm >= goBackIcon.x and xm <= goBackIcon.x + goBackIcon.w and ym >= goBackIcon.y and ym <= goBackIcon.y + goBackIcon.h)
        {
            show = false;
        }
        else if (xm >= img_rect_penChoosingOption.x and xm <= img_rect_penChoosingOption.x + img_rect_penChoosingOption.w
            and ym >= img_rect_penChoosingOption.y and ym <= img_rect_penChoosingOption.y + img_rect_penChoosingOption.h)
        {
            show = false;
            activatePenIcon = true;
        }
    }
    SDL_SetRenderTarget (m_renderer, nullptr);
    SDL_RenderCopy (m_renderer, extensionMenuScreen, nullptr, nullptr);
}

#endif //ADD_EXTENSION_PEN_H