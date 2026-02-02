
#ifndef MOTION_BLOCK_H
#define MOTION_BLOCK_H

void moveSteps (Sprite &sprite, int n, SDL_Renderer *m_renderer)
{
    if (sprite.x + sprite.img_rect.w / 2 <= 1490 and sprite.x - sprite.img_rect.w / 2 >= 870
        and sprite.y - sprite.img_rect.h / 2 >= 120 and sprite.y + sprite.img_rect.h / 2 <= 600)
    {
        sprite.x += 2 * cos (sprite.direction * M_PI / 180.0) * n;
        sprite.y += 2 * sin (sprite.direction * M_PI / 180.0) * n;
    }
    sprite.draw (m_renderer);
}

void turnRight (Sprite &sprite, int degree, SDL_Renderer *m_renderer)
{
    sprite.direction += degree;
    sprite.draw (m_renderer);
}

void turnLeft (Sprite &sprite, int degree, SDL_Renderer *m_renderer)
{
    sprite.direction -= degree;
    sprite.draw (m_renderer);
}

void goToCoords (Sprite &sprite, SDL_Renderer *m_renderer,bool mouse, bool random = false, int x = 0, int y = 0)
{
    if (!random)
    {
        x += 1180, y = -y , y += 360;
        if (x >= 1490) {sprite.x = 1490 - sprite.img_rect.w / 2;}
        else if (x <= 870) {sprite.x = 870 + sprite.img_rect.w / 2;}
        if (y >= 600) {sprite.y = 600 - sprite.img_rect.h / 2;}
        else if (y <= 120) {sprite.y = 120 + sprite.img_rect.h / 2;}
        if (x <= 1490 and x >= 870 and y >= 120 and y <= 600) {sprite.x = x, sprite.y = y;}
    }
    else
    {
        srand (time(nullptr));
        x = rand () % (621 - sprite.img_rect.w)+ 870 + sprite.img_rect.w / 2;
        srand (time(nullptr));
        y = rand () % (481 - sprite.img_rect.h) + 120 + sprite.img_rect.h / 2;
        sprite.x = x, sprite.y = y;
    }
    if (mouse)
    {
        int xm, ym;
        SDL_GetMouseState (&xm, &ym);
        if (xm >= 1490) {sprite.x = 1490 - sprite.img_rect.w / 2;}
        else if (xm <= 870) {sprite.x = 870 + sprite.img_rect.w / 2;}
        if (ym >= 600) {sprite.y = 600 - sprite.img_rect.h / 2;}
        else if (ym <= 120) {sprite.y = 120 + sprite.img_rect.h / 2;}
        if (xm <= 1490 and xm >= 870 and ym >= 120 and ym <= 600) {sprite.x = xm, sprite.y = ym;}
    }
    sprite.draw (m_renderer);
}

void changeCoordsBy (Sprite &sprite, SDL_Renderer *m_renderer, int x = 0, int y = 0)
{
    bool f = true;
    if (sprite.x + sprite.img_rect.w / 2 + x >= 1490) {sprite.x = 1490 - sprite.img_rect.w / 2, f = false;}
    else if (sprite.x - sprite.img_rect.w / 2 + x <= 870) {sprite.x = 870 + sprite.img_rect.w / 2, f = false;}
    if (sprite.y + sprite.img_rect.h / 2 - y >= 600) {sprite.y = 600 - sprite.img_rect.h / 2, f = false;}
    else if (sprite.y - sprite.img_rect.h / 2 - y <= 120) {sprite.y = 120 + sprite.img_rect.h / 2, f = false;}
    if (f) {sprite.x += x, sprite.y += y;}
    sprite.draw (m_renderer);
}

void pointInDirection (Sprite &sprite, int degree, SDL_Renderer *m_renderer)
{
    sprite.direction = degree - 90;
    sprite.draw (m_renderer);
}

#endif //MOTION_BLOCK_H