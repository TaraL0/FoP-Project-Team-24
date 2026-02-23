#ifndef COSTUME_MENU_H
#define COSTUME_MENU_H

enum class PaintTool
{
    PEN,
    BUCKET,
    ERASER,
    CLEAR,
    SHAPE,
    TEXT,
    POSITION
};

enum class ShapeKind
{
    RECTANGLE,
    ELLIPSE,
    LINE
};


struct UndoStep
{
    std::vector<Uint32> pixels;
    int w, h;
};


struct CostumeEditor
{
    bool            isOpen           = false;
    PaintTool       currentTool      = PaintTool::PEN;
    ShapeKind       currentShape     = ShapeKind::RECTANGLE;
    SDL_Color       brushColor       = {0, 0, 0, 255};
    int             brushSize        = 3;
    int             eraserSize       = 12;
    float           zoom             = 4.0f;
    SDL_Point       panOffset        = {0, 0};

    SDL_Texture    *canvasTex        = nullptr;
    int             canvasW          = 0;
    int             canvasH          = 0;
    std::vector<Uint32> canvasPixels;
    std::vector<Uint32> originalPixels;
    Sprite         *targetSprite     = nullptr;

    bool            drawing          = false;
    int             lastMX           = -1, lastMY = -1;
    int             shapeStartX      = -1, shapeStartY = -1;
    bool            shapeActive      = false;
    std::vector<Uint32> shapeSnapshot;

    bool            textMode         = false;
    std::string     textBuffer;
    int             textCursorX      = 0, textCursorY = 0;
    int             textFontSize     = 16;

    std::vector<UndoStep> undoStack;
    std::vector<UndoStep> redoStack;
    static const int MAX_UNDO = 40;

    SDL_Rect        editingPanel     = {145, 120, 710, 900};
    SDL_Rect        toolbarRect      = {145, 120, 50,  900};
    SDL_Rect        canvasViewport   = {200, 130, 645, 880};
    SDL_Rect        colorBarRect     = {200, 1015, 645, 30};

    struct ToolBtn { SDL_Rect r; PaintTool tool; const char* label; };
    std::vector<ToolBtn> toolButtons;

    bool            showShapeMenu    = false;
    struct ShapeBtn { SDL_Rect r; ShapeKind kind; const char* label; };
    std::vector<ShapeBtn> shapeButtons;

    struct PaletteColor { SDL_Color c; SDL_Rect r; };
    std::vector<PaletteColor> palette;

    TTF_Font       *font             = nullptr;

    void init (TTF_Font *f)
    {
        font = f;
        buildToolbar ();
        buildPalette ();
    }

    void buildToolbar ()
    {
        toolButtons.clear ();
        const char* labels[] = {"Pen","Bkt","Ers","Clr","Shp","Txt","Pos"};
        PaintTool   tools[]  = {PaintTool::PEN, PaintTool::BUCKET,
                                PaintTool::ERASER, PaintTool::CLEAR,
                                PaintTool::SHAPE, PaintTool::TEXT,
                                PaintTool::POSITION};
        int bx = toolbarRect.x + 2;
        int by = toolbarRect.y + 10;
        for (int i = 0; i < 7; i++)
        {
            SDL_Rect r = {bx, by + i * 62, 46, 52};
            toolButtons.push_back ({r, tools[i], labels[i]});
        }

        shapeButtons.clear ();
        const char* sLabels[] = {"Rect","Ellip","Line","Tri"};
        ShapeKind   sKinds[]  = {ShapeKind::RECTANGLE, ShapeKind::ELLIPSE,
                                 ShapeKind::LINE};
        for (int i = 0; i < 3; i++)
        {
            SDL_Rect r = {bx + 52, toolButtons[4].r.y + i * 34, 60, 30};
            shapeButtons.push_back ({r, sKinds[i], sLabels[i]});
        }
    }

    void buildPalette ()
    {
        palette.clear ();
        SDL_Color cols[] = {
            {0,0,0,255},{255,255,255,255},{255,0,0,255},{0,255,0,255},
            {0,0,255,255},{255,255,0,255},{255,0,255,255},{0,255,255,255},
            {128,0,0,255},{0,128,0,255},{0,0,128,255},{128,128,0,255},
            {128,0,128,255},{0,128,128,255},{128,128,128,255},{255,128,0,255},
            {255,200,200,255},{200,200,255,255}
        };
        int n = sizeof(cols)/sizeof(cols[0]);
        int sw = colorBarRect.w / n;
        for (int i = 0; i < n; i++)
        {
            SDL_Rect r = {colorBarRect.x + i * sw, colorBarRect.y, sw, colorBarRect.h};
            palette.push_back ({cols[i], r});
        }
    }

    void open (SDL_Renderer *renderer, Sprite &sprite)
    {
        targetSprite = &sprite;
        isOpen = true;
        textMode = false;
        textBuffer.clear ();
        drawing = false;
        shapeActive = false;
        undoStack.clear ();
        redoStack.clear ();

        if (sprite.costumes.empty ()) return;
        SDL_Texture *src = sprite.costumes[sprite.curCostumeNum];
        SDL_QueryTexture (src, nullptr, nullptr, &canvasW, &canvasH);

        canvasPixels.resize (canvasW * canvasH);

        SDL_Texture *prev = SDL_GetRenderTarget (renderer);
        SDL_Texture *tmp  = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_ARGB8888,
                                               SDL_TEXTUREACCESS_TARGET, canvasW, canvasH);
        SDL_SetRenderTarget (renderer, tmp);
        SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0);
        SDL_RenderClear (renderer);
        SDL_RenderCopy (renderer, src, nullptr, nullptr);

        SDL_Rect rr = {0, 0, canvasW, canvasH};
        SDL_RenderReadPixels (renderer, &rr, SDL_PIXELFORMAT_ARGB8888,
                              canvasPixels.data (), canvasW * 4);

        SDL_SetRenderTarget (renderer, prev);
        SDL_DestroyTexture (tmp);

        if (canvasTex) SDL_DestroyTexture (canvasTex);
        canvasTex = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       canvasW, canvasH);
        SDL_SetTextureBlendMode (canvasTex, SDL_BLENDMODE_BLEND);
        uploadPixels ();
        pushUndo ();
        originalPixels = canvasPixels;

        float zx = (float)canvasViewport.w / (float)canvasW;
        float zy = (float)canvasViewport.h / (float)canvasH;
        zoom = std::min (zx, zy) * 0.9f;
        panOffset = {0, 0};
    }

    void close ()
    {
        isOpen = false;
        if (canvasTex) { SDL_DestroyTexture (canvasTex); canvasTex = nullptr; }
    }

    void commitToSprite (SDL_Renderer *renderer)
    {
        if (!targetSprite || targetSprite->costumes.empty ()) return;
        int idx = targetSprite->curCostumeNum;

        SDL_DestroyTexture (targetSprite->costumes[idx]);
        SDL_Texture *neo = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, canvasW, canvasH);
        SDL_SetTextureBlendMode (neo, SDL_BLENDMODE_BLEND);

        SDL_Texture *stm = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, canvasW, canvasH);
        void *px; int pitch;
        SDL_LockTexture (stm, nullptr, &px, &pitch);
        std::memcpy (px, canvasPixels.data (), canvasW * canvasH * 4);
        SDL_UnlockTexture (stm);

        SDL_Texture *prev = SDL_GetRenderTarget (renderer);
        SDL_SetRenderTarget (renderer, neo);
        SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0);
        SDL_RenderClear (renderer);
        SDL_RenderCopy (renderer, stm, nullptr, nullptr);
        SDL_SetRenderTarget (renderer, prev);
        SDL_DestroyTexture (stm);

        targetSprite->costumes[idx] = neo;
        targetSprite->costumesWidth[idx]  = canvasW;
        targetSprite->costumesHeight[idx] = canvasH;
    }

    inline Uint32 colorToPixel (SDL_Color c)
    {
        return ((Uint32)c.a << 24) | ((Uint32)c.r << 16) |
               ((Uint32)c.g << 8)  |  (Uint32)c.b;
    }
    inline SDL_Color pixelToColor (Uint32 p)
    {
        SDL_Color c;
        c.a = (p >> 24) & 0xFF;
        c.r = (p >> 16) & 0xFF;
        c.g = (p >>  8) & 0xFF;
        c.b =  p        & 0xFF;
        return c;
    }

    bool inCanvas (int cx, int cy)
    {
        return cx >= 0 && cy >= 0 && cx < canvasW && cy < canvasH;
    }

    void setPixel (int cx, int cy, SDL_Color c)
    {
        if (!inCanvas (cx, cy)) return;
        canvasPixels[cy * canvasW + cx] = colorToPixel (c);
    }

    Uint32 getPixel (int cx, int cy)
    {
        if (!inCanvas (cx, cy)) return 0;
        return canvasPixels[cy * canvasW + cx];
    }

    bool screenToCanvas (int sx, int sy, int &cx, int &cy)
    {
        float ox = canvasViewport.x + canvasViewport.w / 2.0f + panOffset.x - (canvasW  * zoom) / 2.0f;
        float oy = canvasViewport.y + canvasViewport.h / 2.0f + panOffset.y - (canvasH * zoom) / 2.0f;
        cx = (int)((sx - ox) / zoom);
        cy = (int)((sy - oy) / zoom);
        return inCanvas (cx, cy);
    }

    void uploadPixels ()
    {
        if (!canvasTex) return;
        void *px; int pitch;
        SDL_LockTexture (canvasTex, nullptr, &px, &pitch);
        for (int row = 0; row < canvasH; row++)
            std::memcpy ((Uint8*)px + row * pitch,
                         canvasPixels.data () + row * canvasW,
                         canvasW * 4);
        SDL_UnlockTexture (canvasTex);
    }

    void pushUndo ()
    {
        UndoStep s;
        s.pixels = canvasPixels;
        s.w = canvasW; s.h = canvasH;
        undoStack.push_back (s);
        if ((int)undoStack.size () > MAX_UNDO)
            undoStack.erase (undoStack.begin ());
        redoStack.clear ();
    }
    void undo ()
    {
        if (undoStack.size () <= 1) return;
        redoStack.push_back (undoStack.back ());
        undoStack.pop_back ();
        canvasPixels = undoStack.back ().pixels;
        uploadPixels ();
    }
    void redo ()
    {
        if (redoStack.empty ()) return;
        undoStack.push_back (redoStack.back ());
        redoStack.pop_back ();
        canvasPixels = undoStack.back ().pixels;
        uploadPixels ();
    }

    void drawBrushDot (int cx, int cy, int size, SDL_Color c)
    {
        int r = size / 2;
        for (int dy = -r; dy <= r; dy++)
            for (int dx = -r; dx <= r; dx++)
                if (dx*dx + dy*dy <= r*r)
                    setPixel (cx + dx, cy + dy, c);
    }

    void drawBrushLine (int x0, int y0, int x1, int y1, int size, SDL_Color c)
    {
        int dx = std::abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs (y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;
        while (true)
        {
            drawBrushDot (x0, y0, size, c);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }

    void floodFill (int sx, int sy, SDL_Color fillC)
    {
        if (!inCanvas (sx, sy)) return;
        Uint32 target = getPixel (sx, sy);
        Uint32 repl   = colorToPixel (fillC);
        if (target == repl) return;

        std::vector<SDL_Point> stack;
        stack.push_back ({sx, sy});
        while (!stack.empty ())
        {
            SDL_Point p = stack.back (); stack.pop_back ();
            if (!inCanvas (p.x, p.y)) continue;
            if (getPixel (p.x, p.y) != target) continue;
            int lx = p.x;
            while (lx > 0 && getPixel (lx - 1, p.y) == target) lx--;
            bool spanAbove = false, spanBelow = false;
            for (int xi = lx; xi < canvasW && getPixel (xi, p.y) == target; xi++)
            {
                canvasPixels[p.y * canvasW + xi] = repl;
                if (!spanAbove && p.y > 0 && getPixel (xi, p.y - 1) == target)
                { stack.push_back ({xi, p.y - 1}); spanAbove = true; }
                else if (spanAbove && p.y > 0 && getPixel (xi, p.y - 1) != target)
                    spanAbove = false;
                if (!spanBelow && p.y < canvasH - 1 && getPixel (xi, p.y + 1) == target)
                { stack.push_back ({xi, p.y + 1}); spanBelow = true; }
                else if (spanBelow && p.y < canvasH - 1 && getPixel (xi, p.y + 1) != target)
                    spanBelow = false;
            }
        }
    }

    void drawRectOutline (int x0, int y0, int x1, int y1, SDL_Color c)
    {
        if (x0 > x1) std::swap (x0, x1);
        if (y0 > y1) std::swap (y0, y1);
        for (int x = x0; x <= x1; x++) { setPixel (x, y0, c); setPixel (x, y1, c); }
        for (int y = y0; y <= y1; y++) { setPixel (x0, y, c); setPixel (x1, y, c); }
    }

    void drawEllipseOutline (int cx, int cy, int rx, int ry, SDL_Color c)
    {
        if (rx <= 0 || ry <= 0) return;
        for (int deg = 0; deg < 360; deg++)
        {
            double rad = deg * M_PI / 180.0;
            int px = cx + (int)(rx * cos (rad));
            int py = cy + (int)(ry * sin (rad));
            setPixel (px, py, c);
        }
    }

    void drawLineDDA (int x0, int y0, int x1, int y1, SDL_Color c)
    {
        drawBrushLine (x0, y0, x1, y1, brushSize, c);
    }

    void stampText (SDL_Renderer *renderer, int cx, int cy, const std::string &txt, SDL_Color c)
    {
        if (!font || txt.empty ()) return;
        SDL_Surface *surf = TTF_RenderText_Blended (font, txt.c_str (), c);
        if (!surf) return;
        SDL_Surface *conv = SDL_ConvertSurfaceFormat (surf, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_FreeSurface (surf);
        if (!conv) return;
        Uint32 *sp = (Uint32*)conv->pixels;
        for (int row = 0; row < conv->h; row++)
            for (int col = 0; col < conv->w; col++)
            {
                Uint32 p = sp[row * (conv->pitch / 4) + col];
                if ((p >> 24) > 20)
                    setPixel (cx + col, cy + row, pixelToColor (p));
            }
        SDL_FreeSurface (conv);
    }

    void clearCanvas ()
    {
        if (!originalPixels.empty ())
        {
            canvasPixels = originalPixels;
        }
        else
        {
            SDL_Color trans = {255, 255, 255, 0};
            std::fill (canvasPixels.begin (), canvasPixels.end (), colorToPixel (trans));
        }
    }

    void handleEvent (SDL_Event &e, SDL_Renderer *renderer)
    {
        if (!isOpen) return;

        int mx, my;
        SDL_GetMouseState (&mx, &my);

        if (e.type == SDL_KEYDOWN)
        {
            auto mod = SDL_GetModState ();
            if ((mod & KMOD_CTRL) && e.key.keysym.sym == SDLK_z)
            {
                if (mod & KMOD_SHIFT) redo (); else undo ();
                commitToSprite (renderer);
                return;
            }
            if (textMode)
            {
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_ESCAPE)
                {
                    if (!textBuffer.empty ())
                    {
                        stampText (renderer, textCursorX, textCursorY, textBuffer, brushColor);
                        uploadPixels ();
                        pushUndo ();
                        commitToSprite (renderer);
                    }
                    textBuffer.clear ();
                    textMode = false;
                    SDL_StopTextInput ();
                }
                else if (e.key.keysym.sym == SDLK_BACKSPACE && !textBuffer.empty ())
                    textBuffer.pop_back ();
                return;
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_PLUS || e.key.keysym.sym == SDLK_EQUALS)
                    brushSize = std::min (brushSize + 1, 50);
                if (e.key.keysym.sym == SDLK_MINUS)
                    brushSize = std::max (brushSize - 1, 1);
            }
        }

        if (e.type == SDL_TEXTINPUT && textMode)
        {
            textBuffer += e.text.text;
            return;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            for (auto &pc : palette)
            {
                if (mx >= pc.r.x && mx <= pc.r.x + pc.r.w &&
                    my >= pc.r.y && my <= pc.r.y + pc.r.h)
                {
                    brushColor = pc.c;
                    return;
                }
            }
            for (auto &tb : toolButtons)
            {
                if (mx >= tb.r.x && mx <= tb.r.x + tb.r.w &&
                    my >= tb.r.y && my <= tb.r.y + tb.r.h)
                {
                    if (tb.tool == PaintTool::CLEAR)
                    {
                        clearCanvas ();
                        uploadPixels ();
                        pushUndo ();
                        commitToSprite (renderer);
                        return;
                    }
                    if (tb.tool == PaintTool::SHAPE)
                        showShapeMenu = !showShapeMenu;
                    else
                        showShapeMenu = false;
                    currentTool = tb.tool;
                    textMode = false;
                    return;
                }
            }
            if (showShapeMenu)
            {
                for (auto &sb : shapeButtons)
                {
                    if (mx >= sb.r.x && mx <= sb.r.x + sb.r.w &&
                        my >= sb.r.y && my <= sb.r.y + sb.r.h)
                    {
                        currentShape = sb.kind;
                        showShapeMenu = false;
                        return;
                    }
                }
            }
        }

        int cx, cy;
        bool onCanvas = screenToCanvas (mx, my, cx, cy);

        if (e.type == SDL_MOUSEWHEEL && onCanvas)
        {
            zoom += e.wheel.y * 0.3f;
            zoom = std::max (0.5f, std::min (zoom, 30.0f));
            return;
        }

        if (e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON_MMASK))
        {
            panOffset.x += e.motion.xrel;
            panOffset.y += e.motion.yrel;
            return;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && onCanvas)
        {
            switch (currentTool)
            {
            case PaintTool::PEN:
                drawing = true;
                drawBrushDot (cx, cy, brushSize, brushColor);
                lastMX = cx; lastMY = cy;
                uploadPixels ();
                break;

            case PaintTool::ERASER:
                drawing = true;
                {
                    SDL_Color trans = {255, 255, 255, 0};
                    drawBrushDot (cx, cy, eraserSize, trans);
                    lastMX = cx; lastMY = cy;
                }
                uploadPixels ();
                break;

            case PaintTool::BUCKET:
                floodFill (cx, cy, brushColor);
                uploadPixels ();
                pushUndo ();
                commitToSprite (renderer);
                break;

            case PaintTool::TEXT:
                textMode = true;
                textCursorX = cx;
                textCursorY = cy;
                textBuffer.clear ();
                SDL_StartTextInput ();
                break;

            case PaintTool::SHAPE:
                shapeActive  = true;
                shapeStartX  = cx;
                shapeStartY  = cy;
                shapeSnapshot = canvasPixels;
                break;

            case PaintTool::POSITION:
                drawing = true;
                lastMX = cx; lastMY = cy;
                break;

            default: break;
            }
        }

        if (e.type == SDL_MOUSEMOTION && drawing && onCanvas)
        {
            switch (currentTool)
            {
            case PaintTool::PEN:
                drawBrushLine (lastMX, lastMY, cx, cy, brushSize, brushColor);
                lastMX = cx; lastMY = cy;
                uploadPixels ();
                break;
            case PaintTool::ERASER:
            {
                SDL_Color trans = {255, 255, 255, 0};
                drawBrushLine (lastMX, lastMY, cx, cy, eraserSize, trans);
                lastMX = cx; lastMY = cy;
                uploadPixels ();
                break;
            }
            case PaintTool::POSITION:
                panOffset.x += (cx - lastMX) * (int)zoom;
                panOffset.y += (cy - lastMY) * (int)zoom;
                lastMX = cx; lastMY = cy;
                break;
            default: break;
            }
        }

        if (e.type == SDL_MOUSEMOTION && shapeActive && currentTool == PaintTool::SHAPE && onCanvas)
        {
            canvasPixels = shapeSnapshot;
            switch (currentShape)
            {
            case ShapeKind::RECTANGLE: drawRectOutline (shapeStartX, shapeStartY, cx, cy, brushColor); break;
            case ShapeKind::ELLIPSE:
            {
                int rx = std::abs (cx - shapeStartX) / 2;
                int ry = std::abs (cy - shapeStartY) / 2;
                int ccx = (shapeStartX + cx) / 2;
                int ccy = (shapeStartY + cy) / 2;
                drawEllipseOutline (ccx, ccy, rx, ry, brushColor);
                break;
            }
            case ShapeKind::LINE:     drawLineDDA (shapeStartX, shapeStartY, cx, cy, brushColor); break;
            }
            uploadPixels ();
        }

        if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
        {
            if (drawing)
            {
                drawing = false;
                pushUndo ();
                commitToSprite (renderer);
            }
            if (shapeActive)
            {
                shapeActive = false;
                pushUndo ();
                commitToSprite (renderer);
            }
        }
    }


    void render (SDL_Renderer *renderer)
    {
        if (!isOpen || !canvasTex) return;

        SDL_SetRenderDrawColor (renderer, 60, 60, 70, 255);
        SDL_RenderFillRect (renderer, &toolbarRect);

        for (auto &tb : toolButtons)
        {
            bool sel = (tb.tool == currentTool);
            if (sel)
                SDL_SetRenderDrawColor (renderer, 100, 140, 255, 255);
            else
                SDL_SetRenderDrawColor (renderer, 90, 90, 100, 255);
            SDL_RenderFillRect (renderer, &tb.r);

            if (font)
            {
                SDL_Color tc = {255, 255, 255, 255};
                SDL_Surface *s = TTF_RenderText_Blended (font, tb.label, tc);
                if (s)
                {
                    SDL_Texture *t = SDL_CreateTextureFromSurface (renderer, s);
                    SDL_Rect dr = {tb.r.x + 3, tb.r.y + 16, s->w, s->h};
                    SDL_RenderCopy (renderer, t, nullptr, &dr);
                    SDL_DestroyTexture (t);
                    SDL_FreeSurface (s);
                }
            }
        }

        if (showShapeMenu)
        {
            for (auto &sb : shapeButtons)
            {
                bool sel2 = (sb.kind == currentShape);
                SDL_SetRenderDrawColor (renderer, sel2 ? 100 : 75, sel2 ? 140 : 75, sel2 ? 255 : 85, 255);
                SDL_RenderFillRect (renderer, &sb.r);
                if (font)
                {
                    SDL_Color tc = {255, 255, 255, 255};
                    SDL_Surface *s = TTF_RenderText_Blended (font, sb.label, tc);
                    if (s)
                    {
                        SDL_Texture *t = SDL_CreateTextureFromSurface (renderer, s);
                        SDL_Rect dr = {sb.r.x + 3, sb.r.y + 5, s->w, s->h};
                        SDL_RenderCopy (renderer, t, nullptr, &dr);
                        SDL_DestroyTexture (t);
                        SDL_FreeSurface (s);
                    }
                }
            }
        }

        SDL_RenderSetClipRect (renderer, &canvasViewport);
        {
            int dw = (int)(canvasW * zoom);
            int dh = (int)(canvasH * zoom);
            int dx = canvasViewport.x + canvasViewport.w / 2 + panOffset.x - dw / 2;
            int dy = canvasViewport.y + canvasViewport.h / 2 + panOffset.y - dh / 2;
            SDL_Rect dst = {dx, dy, dw, dh};

            int sq = std::max (4, (int)(8 * zoom));
            for (int row = 0; row * sq < dh; row++)
                for (int col = 0; col * sq < dw; col++)
                {
                    bool dark = (row + col) % 2 == 0;
                    SDL_SetRenderDrawColor (renderer, dark ? 200 : 240,
                                            dark ? 200 : 240, dark ? 200 : 240, 255);
                    SDL_Rect sq_r = {dx + col * sq, dy + row * sq,
                                     sq, sq};
                    SDL_RenderFillRect (renderer, &sq_r);
                }

            SDL_RenderCopy (renderer, canvasTex, nullptr, &dst);
        }
        SDL_RenderSetClipRect (renderer, nullptr);

        for (auto &pc : palette)
        {
            SDL_SetRenderDrawColor (renderer, pc.c.r, pc.c.g, pc.c.b, 255);
            SDL_RenderFillRect (renderer, &pc.r);
            // highlight selected
            if (pc.c.r == brushColor.r && pc.c.g == brushColor.g &&
                pc.c.b == brushColor.b && pc.c.a == brushColor.a)
            {
                SDL_SetRenderDrawColor (renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect (renderer, &pc.r);
            }
        }

        if (font)
        {
            char buf[32];
            snprintf (buf, sizeof(buf), "Size:%d  Zoom:%.1fx", brushSize, zoom);
            SDL_Color tc = {50, 50, 50, 255};
            SDL_Surface *s = TTF_RenderText_Blended (font, buf, tc);
            if (s)
            {
                SDL_Texture *t = SDL_CreateTextureFromSurface (renderer, s);
                SDL_Rect dr = {canvasViewport.x, canvasViewport.y - 18, s->w, s->h};
                SDL_RenderCopy (renderer, t, nullptr, &dr);
                SDL_DestroyTexture (t);
                SDL_FreeSurface (s);
            }
        }

        if (textMode && font && !textBuffer.empty ())
        {
            SDL_Color tc = {0, 0, 0, 255};
            std::string display = textBuffer + "_";
            SDL_Surface *s = TTF_RenderText_Blended (font, display.c_str (), tc);
            if (s)
            {
                SDL_Texture *t = SDL_CreateTextureFromSurface (renderer, s);
                SDL_Rect dr = {canvasViewport.x + 10, canvasViewport.y + canvasViewport.h + 5, s->w, s->h};
                SDL_RenderCopy (renderer, t, nullptr, &dr);
                SDL_DestroyTexture (t);
                SDL_FreeSurface (s);
            }
        }
    }

    void destroy ()
    {
        close ();
    }
};

CostumeEditor costumeEditor;

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
        {show = true; if (!costumeEditor.isOpen)
            costumeEditor.open(m_renderer, sprite);}
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
    SDL_Rect uploadIcon = {22, 200, 100, 60};
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
        textRect = {38, 217, textSurface -> w, textSurface -> h};
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
            costumeEditor.close();
        }
        else if (xm >= uploadIcon.x and xm <= uploadIcon.x + uploadIcon.w and ym >= uploadIcon.y and ym <= uploadIcon.y + uploadIcon.h)
        {
            uploadImage (m_renderer, sprite);
        }
    }
    SDL_SetRenderTarget (m_renderer, nullptr);
    SDL_RenderCopy (m_renderer, costumeMenuScreen, nullptr, nullptr);
    costumeEditor.render(m_renderer);
}


#endif // COSTUME_MENU_H
