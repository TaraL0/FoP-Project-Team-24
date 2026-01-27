
#ifndef MOTION_BLOCK_H
#define MOTION_BLOCK_H

void moveSteps (Sprite &sprite, int n, SDL_Renderer *m_renderer)
{
    sprite.img_rect.x += n * 2;
    sprite.x += n * 2;
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.direction, &center, SDL_FLIP_NONE);
}

void turnRight (Sprite &sprite, int degree, SDL_Renderer *m_renderer)
{
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    sprite.direction += degree;
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.direction, &center, SDL_FLIP_NONE);
}

void turnLeft (Sprite &sprite, int degree, SDL_Renderer *m_renderer)
{
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    sprite.direction -= degree;
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.direction, &center, SDL_FLIP_NONE);
}

// can be used for "go to random position / mouse pointer" too
void goToCoords (Sprite &sprite, int x, int y, SDL_Renderer *m_renderer)
{
    sprite.img_rect.x = x, sprite.img_rect.y = y, sprite.x = x, sprite.y = y;
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.direction, &center, SDL_FLIP_NONE);
}

void changeCoordsBy (Sprite &sprite, int x, int y, SDL_Renderer *m_renderer)
{
    sprite.img_rect.x += x, sprite.img_rect.y = +y, sprite.x += x, sprite.y += y;
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.direction, &center, SDL_FLIP_NONE);
}

void pointInDirection (Sprite &sprite, int degree, SDL_Renderer *m_renderer)
{
    SDL_Point center;
    center.x = sprite.w / 2, center.y = sprite.h / 2;
    sprite.direction = degree - 90;
    SDL_RenderCopyEx (m_renderer, sprite.m_img, &sprite.img_src, &sprite.img_rect, sprite.direction, &center, SDL_FLIP_NONE);
}

#endif //MOTION_BLOCK_H