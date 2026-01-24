
#ifndef MOTION_BLOCK_H
#define MOTION_BLOCK_H

#include "sprite.h"

using namespace std;

void moveSteps (Sprite &sprite, int n, SDL_Renderer *m_renderer)
{
    sprite.img_rect.x += n * 2;
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    SDL_RenderClear (m_renderer);
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.angle, &center, SDL_FLIP_NONE);
    SDL_RenderPresent (m_renderer);
}

void turnRight (Sprite &sprite, int degree, SDL_Renderer *m_renderer)
{
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    sprite.angle += degree;
    SDL_RenderClear (m_renderer);
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.angle, &center, SDL_FLIP_NONE);
    SDL_RenderPresent (m_renderer);
}

void turnLeft (Sprite &sprite, int degree, SDL_Renderer *m_renderer)
{
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    sprite.angle -= degree;
    SDL_RenderClear (m_renderer);
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.angle, &center, SDL_FLIP_NONE);
    SDL_RenderPresent (m_renderer);
}

// can be used for "go to random position / mouse pointer" too
void goToCoords (Sprite &sprite, int x, int y, SDL_Renderer *m_renderer)
{
    sprite.img_rect.x = x, sprite.img_rect.y = y;
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    SDL_RenderClear (m_renderer);
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.angle, &center, SDL_FLIP_NONE);
    SDL_RenderPresent (m_renderer);
}

void changeCoordsBy (Sprite &sprite, int x, int y, SDL_Renderer *m_renderer)
{
    sprite.img_rect.x += x, sprite.img_rect.y = +y;
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    SDL_RenderClear (m_renderer);
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.angle, &center, SDL_FLIP_NONE);
    SDL_RenderPresent (m_renderer);
}

void pointInDirection (Sprite &sprite, int degree, SDL_Renderer *m_renderer)
{
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    sprite.angle = degree - 90;
    SDL_RenderClear (m_renderer);
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.angle, &center, SDL_FLIP_NONE);
    SDL_RenderPresent (m_renderer);
}

#endif //MOTION_BLOCK_H