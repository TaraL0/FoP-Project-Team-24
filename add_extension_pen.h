
#ifndef ADD_EXTENSION_PEN_H
#define ADD_EXTENSION_PEN_H

struct Pen
{
    bool penDown = false;
    SDL_Color penColor = {0, 255, 0, 255};
    int penWidth = 1;

    void draw (SDL_Renderer* m_renderer, Sprite &sprite)
    {
        for (int dx = -penWidth / 2; dx <= penWidth / 2; dx ++)
            for (int dy = -penWidth / 2; dy <= penWidth / 2; dy ++)
                SDL_RenderDrawLine (m_renderer, );
    }

    void penUp () {penDown = false;}

    void eraseAll () {}

    void stamp () {}

    void changePen (string value, double num)
    {
        if (value == "color") {}
        else if (value == "saturation") {}
        else if (value == "brightness") {}
    }

    void setPen (string value, double num)
    {
        if (value == "color") {}
        else if (value == "saturation") {}
        else if (value == "brightness") {}
    }

    void setPenColor (SDL_Color color) {penColor = color;}

    void setPenWidth (int width) {penWidth = width;}

    void changePenWidth (int width) {penWidth += width;}
};

#endif //ADD_EXTENSION_PEN_H