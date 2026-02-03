
#ifndef LOOKS_BLOCK_H
#define LOOKS_BLOCK_H

void sayText (Sprite &sprite, string text, TTF_Font *font, SDL_Renderer *m_renderer)
{
    if (font)
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, text.c_str (), textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_SetRenderDrawColor (m_renderer, 255, 255, 255, 255);
        SDL_Rect bubble = {sprite.img_rect.x + sprite.img_rect.w - 8, sprite.img_rect.y - (textSurface -> h + 16),
            textSurface -> w + 16, textSurface -> h + 16};
        SDL_RenderFillRect (m_renderer, &bubble);
        SDL_SetRenderDrawColor (m_renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect (m_renderer, &bubble);
        SDL_Rect textRect = {bubble.x + 8, bubble.y + 8, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
}

void sayTextForSec (Sprite &sprite, string text, int seconds, TTF_Font *font, SDL_Renderer *m_renderer)
{
    if (font)
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, text.c_str (), textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_SetRenderDrawColor (m_renderer, 255, 255, 255, 255);
        SDL_Rect bubble = {sprite.img_rect.x + sprite.img_rect.w - 8, sprite.img_rect.y - (textSurface -> h + 16),
            textSurface -> w + 16, textSurface -> h + 16};
        SDL_RenderFillRect (m_renderer, &bubble);
        SDL_SetRenderDrawColor (m_renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect (m_renderer, &bubble);
        SDL_Rect textRect = {bubble.x + 8, bubble.y + 8, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
    SDL_RenderPresent (m_renderer);
    SDL_Delay (seconds * 1000);
}

void thinkText (Sprite &sprite, string text, TTF_Font *font, SDL_Renderer *m_renderer)
{
    if (font)
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, text.c_str (), textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_Rect bubble = {sprite.img_rect.x + sprite.img_rect.w / 2 - 14 - (textSurface -> w / 2), sprite.img_rect.y - (textSurface -> h + 28),
            textSurface -> w + 28, textSurface -> h + 28};
        filledEllipseRGBA (m_renderer, sprite.img_rect.x + sprite.img_rect.w / 2, sprite.img_rect.y - (textSurface -> h / 2) - 14,
            (textSurface -> w / 2) + 14, (textSurface -> h / 2) + 14, 180, 180, 180, 255);
        SDL_SetRenderDrawColor (m_renderer, 0, 0, 0, 255);
        SDL_Rect textRect = {bubble.x + 14, bubble.y + 14, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
}

void thinkTextForSec (Sprite &sprite, string text, int seconds, TTF_Font *font, SDL_Renderer *m_renderer)
{
    if (font)
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, text.c_str (), textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_Rect bubble = {sprite.img_rect.x + sprite.img_rect.w / 2 - 14 - (textSurface -> w / 2), sprite.img_rect.y - (textSurface -> h + 28),
            textSurface -> w + 28, textSurface -> h + 28};
        filledEllipseRGBA (m_renderer, sprite.img_rect.x + sprite.img_rect.w / 2, sprite.img_rect.y - (textSurface -> h / 2) - 14,
            (textSurface -> w / 2) + 14, (textSurface -> h / 2) + 14, 180, 180, 180, 255);
        SDL_SetRenderDrawColor (m_renderer, 0, 0, 0, 255);
        SDL_Rect textRect = {bubble.x + 14, bubble.y + 14, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
    SDL_RenderPresent (m_renderer);
    SDL_Delay (seconds * 1000);
}

void switchCostume (Sprite &sprite, string name, SDL_Renderer *m_renderer)
{
    bool found = false;
    for (int i = 0; i < sprite.costumes.size (); i ++)
    {
        if (sprite.costumesName [i] == name) {sprite.curCostumeNum = i; found = true; break;}
    }
    if (!found) {return;}
}

void nextCostume (Sprite &sprite, SDL_Renderer *m_renderer)
{
    if (sprite.curCostumeNum != sprite.costumes.size () - 1) {sprite.curCostumeNum ++;}
    else {sprite.curCostumeNum = 0;}
}

void switchBackdrop (Stage &stage, string name, SDL_Renderer *m_renderer, SDL_Rect mainStage) // !!!!!!!!!!!!!!!!!!!!!!!!!
{
    bool found = false;
    for (int i = 0; i < stage.backdrops.size (); i ++)
    {
        if (stage.backdropsName [i] == name) {stage.curBackdropNum = i; found = true; break;}
    }
    if (!found) {return;}
}

void nextBackdrop (Stage &stage, SDL_Renderer *m_renderer, SDL_Rect mainStage) // !!!!!!!!!!!!!!!!!!!!!!!!!!
{
    if (stage.curBackdropNum != stage.backdrops.size () - 1) {stage.curBackdropNum ++;}
    else {stage.curBackdropNum = 0;}
}

void changeSize (Sprite &sprite, double size, SDL_Renderer *m_renderer)
{
    if (sprite.scale + size >= 300) {sprite.scale = 300;}
    else if (sprite.scale + size <= 10) {sprite.scale = 5;}
    else {sprite.scale += size;}
}

void setSize (Sprite &sprite, double size, SDL_Renderer *m_renderer)
{
    if (size >= 300) {sprite.scale = 300;}
    else if (size <= 10) {sprite.scale = 5;}
    else {sprite.scale = size;}
}

void showSprite (Sprite &sprite) {sprite.visible = true;}

void hideSprite (Sprite &sprite) {sprite.visible = false;}

void goToLayer (Sprite &sprite, string value) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
    if (value == "front") {}
    else if (value == "back") {}
}

void goLayers (Sprite &sprite, string value, int layerNum) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
    if (value == "forward") {}
    else if (value == "backward") {}
}

void costumeReport (Sprite &sprite, string input, SDL_Renderer *m_renderer, TTF_Font *font)
{
    string output;
    if (input == "number")
    {
        output = sprite.mainName + " : costume number = " + to_string (sprite.curCostumeNum + 1);
        if (font)
        {
            SDL_Color textColor = {255, 0, 0};
            SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
            SDL_Rect textRect = {875, 135, textSurface -> w, textSurface -> h};
            SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
            SDL_Rect displaySize = {textRect.x - 2, textRect.y - 2, textRect.w + 4, textRect.h + 4};
            SDL_RenderFillRect (m_renderer, &displaySize);
            SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture (textTexture);
            SDL_FreeSurface (textSurface);
        }
    }
    else if (input == "name")
    {
        output = sprite.mainName + " : costume name ( " + sprite.costumesName [sprite.curCostumeNum] + " )";
        if (font)
        {
            SDL_Color textColor = {255, 0, 0};
            SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
            SDL_Rect textRect = {875, 135, textSurface -> w, textSurface -> h};
            SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
            SDL_Rect displaySize = {textRect.x - 2, textRect.y - 2, textRect.w + 4, textRect.h + 4};
            SDL_RenderFillRect (m_renderer, &displaySize);
            SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture (textTexture);
            SDL_FreeSurface (textSurface);
        }
    }
}

void backdropReport (Stage &stage, string input, SDL_Renderer *m_renderer, TTF_Font *font) // !!!!!!!!!!!!!!!!!!!!!!
{
    string output;
    if (input == "number")
    {
        output =  "backdrop number = " + to_string (stage.curBackdropNum + 1);
        if (font)
        {
            SDL_Color textColor = {0, 150, 0};
            SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
            SDL_Rect textRect = {875, 165, textSurface -> w, textSurface -> h};
            SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
            SDL_Rect displaySize = {textRect.x - 2, textRect.y - 2, textRect.w + 4, textRect.h + 4};
            SDL_RenderFillRect (m_renderer, &displaySize);
            SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture (textTexture);
            SDL_FreeSurface (textSurface);
        }
    }
    else if (input == "name")
    {
        output = "backdrop name ( " + stage.backdropsName [stage.curBackdropNum] + " )";
        if (font)
        {
            SDL_Color textColor = {0, 150, 0};
            SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
            SDL_Rect textRect = {875, 165, textSurface -> w, textSurface -> h};
            SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
            SDL_Rect displaySize = {textRect.x - 2, textRect.y - 2, textRect.w + 4, textRect.h + 4};
            SDL_RenderFillRect (m_renderer, &displaySize);
            SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture (textTexture);
            SDL_FreeSurface (textSurface);
        }
    }
}

void sizeReport (Sprite &sprite, SDL_Renderer *m_renderer,TTF_Font *font)
{
    string output;
    output = sprite.mainName + " : size = " + to_string (int (round (sprite.scale)));
    if (font)
    {
        SDL_Color textColor = {0, 0, 150};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_Rect textRect = {875, 195, textSurface -> w, textSurface -> h};
        SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
        SDL_Rect displaySize = {textRect.x - 2, textRect.y - 2, textRect.w + 4, textRect.h + 4};
        SDL_RenderFillRect (m_renderer, &displaySize);
        SDL_RenderCopy (m_renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
}

#endif //LOOKS_BLOCK_H