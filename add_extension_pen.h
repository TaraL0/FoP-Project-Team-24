
#ifndef ADD_EXTENSION_PEN_H
#define ADD_EXTENSION_PEN_H

struct Pen
{
    bool penDown = false;
    SDL_Color penColor = {0, 255, 0, 255};
    int penWidth = 1;

    void draw (SDL_Renderer* m_renderer, Sprite &sprite)
    {
        if (!penDown) return;
        thickLineRGBA (m_renderer, sprite.x, sprite.y, sprite.x + 1, sprite.y + 1,
            penWidth, penColor.r, penColor.g, penColor.b, penColor.a);
    }

    void penUp () {penDown = false;}

    void eraseAll () {}

    void stamp () {}

    void setPenColor (SDL_Color color) {penColor = color;}

    void setPenWidth (int width) {penWidth = width;}

    void changePenWidth (int width) {penWidth += width;}
};

#endif //ADD_EXTENSION_PEN_H