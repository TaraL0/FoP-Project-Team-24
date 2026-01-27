
#ifndef LOOKS_BLOCK_H
#define LOOKS_BLOCK_H

void myDisplay1 (SDL_Renderer *m_renderer, Sprite &sprite)
{
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    SDL_RenderCopyEx (m_renderer, sprite.costumes [sprite.curCostumeNum], &sprite.img_src, &sprite.img_rect, sprite.direction, &center, SDL_FLIP_NONE);
}

void myDisplay2 (SDL_Renderer *m_renderer, Stage &stage, SDL_Rect mainStage)
{
    SDL_RenderCopy (m_renderer, stage.backdrops [stage.curBackdropNum], NULL, &mainStage);
}

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
        SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
    SDL_Delay (seconds * 1000);
    myDisplay1 (m_renderer, sprite);
}

void thinkText (Sprite &sprite, string text, TTF_Font *font, SDL_Renderer *m_renderer)
{
    if (font)
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, text.c_str (), textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_Rect bubble = {sprite.img_rect.x + sprite.img_rect.w / 2 - 8 - (textSurface -> w / 2), sprite.img_rect.y - (textSurface -> h + 16),
            textSurface -> w + 16, textSurface -> h + 16};
        filledEllipseRGBA (m_renderer, sprite.img_rect.x + sprite.img_rect.w / 2, sprite.img_rect.y - (textSurface -> h / 2) - 8,
            (textSurface -> w / 2) + 8, (textSurface -> h / 2) + 8, 180, 180, 180, 255);
        SDL_SetRenderDrawColor (m_renderer, 0, 0, 0, 255);
        SDL_Rect textRect = {bubble.x + 8, bubble.y + 8, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
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
        SDL_Rect bubble = {sprite.img_rect.x + sprite.img_rect.w / 2 - 8 - (textSurface -> w / 2), sprite.img_rect.y - (textSurface -> h + 16),
            textSurface -> w + 16, textSurface -> h + 16};
        filledEllipseRGBA (m_renderer, sprite.img_rect.x + sprite.img_rect.w / 2, sprite.img_rect.y - (textSurface -> h / 2) - 8,
            (textSurface -> w / 2) + 8, (textSurface -> h / 2) + 8, 180, 180, 180, 255);
        SDL_SetRenderDrawColor (m_renderer, 0, 0, 0, 255);
        SDL_Rect textRect = {bubble.x + 8, bubble.y + 8, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
    SDL_Delay (seconds * 1000);
    myDisplay1 (m_renderer, sprite);
}

void switchCostume (Sprite &sprite, string name, SDL_Renderer *m_renderer)
{
    bool found = false;
    for (int i = 0; i < sprite.costumes.size (); i ++)
    {
        if (sprite.costumesName [i] == name) {sprite.curCostumeNum = i; myDisplay1 (m_renderer, sprite); found = true; break;}
    }
    if (!found) {return;}
}

void nextCostume (Sprite &sprite, SDL_Renderer *m_renderer)
{
    if (sprite.curCostumeNum != sprite.costumes.size () - 1) {sprite.curCostumeNum ++;}
    else {sprite.curCostumeNum = 0;}
    myDisplay1 (m_renderer, sprite);
}

void switchBackdrop (Stage &stage, string name, SDL_Renderer *m_renderer, SDL_Rect mainStage)
{
    bool found = false;
    for (int i = 0; i < stage.backdrops.size (); i ++)
    {
        if (stage.backdropsName [i] == name) {stage.curBackdropNum = i; myDisplay2 (m_renderer, stage, mainStage); found = true; break;}
    }
    if (!found) {return;}
}

void nextBackdrop (Stage &stage, SDL_Renderer *m_renderer, SDL_Rect mainStage)
{
    if (stage.curBackdropNum != stage.backdrops.size () - 1) {stage.curBackdropNum ++;}
    else {stage.curBackdropNum = 0;}
    myDisplay2 (m_renderer, stage, mainStage);
}

void changeSize (Sprite &sprite, double size, SDL_Renderer *m_renderer)
{
    sprite.scale += size;
    myDisplay1 (m_renderer, sprite);
}

void setSize (Sprite &sprite, double size, SDL_Renderer *m_renderer)
{
    sprite.scale = size;
    myDisplay1 (m_renderer, sprite);
}

void setEffect (Sprite &sprite, string effect, double value)
{
    ;
}

void changeEffect (Sprite &sprite, string effect, double value)
{
    ;
}

void clearEffect (Sprite &sprite) {sprite.curCostumeNum = 0;}

void showSprite (Sprite &sprite, SDL_Renderer *m_renderer) {sprite.visible = true; myDisplay1 (m_renderer, sprite);}

void hideSprite (Sprite &sprite) {sprite.visible = false;}

void goToLayer (Sprite &sprite, string value)
{
    if (value == "front") {}
    else if (value == "back") {}
}

void goLayers (Sprite &sprite, string value, int layerNum)
{
    if (value == "forward") {}
    else if (value == "backward") {}
}

void costumeReport (Sprite &sprite, string input, SDL_Renderer *m_renderer, TTF_Font *font)
{
    string output;
    if (input == "number")
    {
        output = sprite.mainName + " : costume number " + to_string (sprite.curCostumeNum);
        SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
        SDL_Rect displaySize = {872, 122, 150, 30};
        SDL_RenderFillRect (m_renderer, &displaySize);
        if (font)
        {
            SDL_Color textColor = {0, 0, 0};
            SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
            SDL_Rect textRect = {875, 135, textSurface -> w, textSurface -> h};
            SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture (textTexture);
            SDL_FreeSurface (textSurface);
        }
    }
    else if (input == "name")
    {
        output = sprite.mainName + " : costume name " + sprite.costumesName [sprite.curCostumeNum];
        SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
        SDL_Rect displaySize = {872, 122, 200, 30};
        SDL_RenderFillRect (m_renderer, &displaySize);
        if (font)
        {
            SDL_Color textColor = {0, 0, 0};
            SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
            SDL_Rect textRect = {875, 135, textSurface -> w, textSurface -> h};
            SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture (textTexture);
            SDL_FreeSurface (textSurface);
        }
    }
}

void backdropReport (Stage &stage, string input, SDL_Renderer *m_renderer, TTF_Font *font)
{
    string output;
    if (input == "number")
    {
        output =  "backdrop number " + to_string (stage.curBackdropNum);
        SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
        SDL_Rect displaySize = {872, 122, 150, 30};
        SDL_RenderFillRect (m_renderer, &displaySize);
        if (font)
        {
            SDL_Color textColor = {0, 0, 0};
            SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
            SDL_Rect textRect = {875, 135, textSurface -> w, textSurface -> h};
            SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture (textTexture);
            SDL_FreeSurface (textSurface);
        }
    }
    else if (input == "name")
    {
        output = "backdrop name " + stage.backdropsName [stage.curBackdropNum];
        SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
        SDL_Rect displaySize = {872, 122, 150, 30};
        SDL_RenderFillRect (m_renderer, &displaySize);
        if (font)
        {
            SDL_Color textColor = {0, 0, 0};
            SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
            SDL_Rect textRect = {875, 135, textSurface -> w, textSurface -> h};
            SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture (textTexture);
            SDL_FreeSurface (textSurface);
        }
    }
}

void sizeReport (Sprite &sprite, SDL_Renderer *m_renderer,TTF_Font *font) {
    string output;
    output = sprite.mainName + " : size " + to_string (sprite.scale);
    SDL_SetRenderDrawColor (m_renderer, 200, 200, 200, 255);
    SDL_Rect displaySize = {872, 122, 100, 30};
    SDL_RenderFillRect (m_renderer, &displaySize);
    if (font)
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderText_Blended (font, output.c_str (), textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface (m_renderer, textSurface);
        SDL_Rect textRect = {875, 135, textSurface -> w, textSurface -> h};
        SDL_RenderCopy (m_renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture (textTexture);
        SDL_FreeSurface (textSurface);
    }
}

#endif //LOOKS_BLOCK_H