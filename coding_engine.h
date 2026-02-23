//
// Created by bartar-camps on 2/20/2026.
//

#ifndef CODING_ENGINE_H
#define CODING_ENGINE_H

struct ArgVal
{
    enum Kind { NUM, STR, BOOL };
    Kind kind = NUM;
    string sVal = "";
    bool bVal = false;
    double asNum () const { return sVal.empty () ? 0 : atof (sVal.c_str ()); }
    int asInt () const { return (int)asNum (); }
};

struct PaletteBlock
{
    string opcode;
    string label;
    SDL_Color color;
    int category;
    int fieldCount () const
    {
        int c = 0;
        for (int i = 0; i < (int)label.size () - 1; i++)
            if (label[i] == '%' && (label[i+1]=='n'||label[i+1]=='s'||label[i+1]=='b'))
                c++;
        return c;
    }
    vector<ArgVal> defaultArgs () const
    {
        vector<ArgVal> v;
        for (int i = 0; i < (int)label.size () - 1; i++)
        {
            if (label[i] != '%') continue;
            ArgVal a;
            if (label[i+1] == 'n') { a.kind = ArgVal::NUM; a.sVal = "10"; }
            else if (label[i+1] == 's') { a.kind = ArgVal::STR; a.sVal = ""; }
            else if (label[i+1] == 'b') { a.kind = ArgVal::BOOL; a.bVal = false; }
            else continue;
            v.push_back (a);
        }
        return v;
    }
};

struct BlockInst
{
    int paletteIdx = -1;
    int x = 0, y = 0;
    int w = 0, h = 30;
    vector<ArgVal> args;
    vector<SDL_Rect> fieldRects;
};

struct ScriptChain
{
    vector<int> blockIds;
};

struct ExecState
{
    int chainIdx = 0;
    int pc = 0;
    bool running = false;
    bool stopped = false;
    int repeatCounter = 0;
    int repeatPC = -1;
    int foreverPC = -1;
    Uint32 waitUntilTick = 0;
};

struct CodingEngine
{
    vector<PaletteBlock> palette;
    vector<BlockInst>    blocks;
    vector<ScriptChain>  chains;
    SDL_Rect paletteRect = {0, 120, 145, 880};
    SDL_Rect scriptRect = {145, 120, 725, 880};
    int palScroll = 0;
    int scrScrollX = 0, scrScrollY = 0;
    int  dragIdx = -1;
    int  dragOffX = 0, dragOffY = 0;
    bool draggingFromPalette = false;
    int  editBlockIdx = -1;
    int  editFieldIdx = -1;
    string editBuf;
    bool editActive = false;
    int curCategory = -1;
    vector<ExecState> execStates;
    bool globalRunning = false;
    bool showPenBlocks = false;
    static SDL_Color cMotion() { return {66, 133, 244, 255}; }
    static SDL_Color cLooks() { return {153, 102, 255, 255}; }
    static SDL_Color cSound() { return {207, 99, 207, 255}; }
    static SDL_Color cEvents() { return {255, 191, 0, 255}; }
    static SDL_Color cControl() { return {255, 171, 25, 255}; }
    static SDL_Color cSensing () { return {92, 177, 214, 255}; }
    static SDL_Color cOperator() { return {89, 192, 89, 255}; }
    static SDL_Color cVariable() { return {255, 140, 26, 255}; }
    static SDL_Color cPen() { return {14, 154, 108, 255}; }
    static SDL_Color cMyBlocks() { return {255, 102, 128, 255}; }
    void setPenVisible (bool visible)
    {
        showPenBlocks = visible;
    }

    void init ()
    {
        palette.clear ();
        palette.push_back ({"move_steps","move %n steps",cMotion(), 0});
        palette.push_back ({"turn_right","turn right %n degrees",cMotion(), 0});
        palette.push_back ({"turn_left","turn left %n degrees",cMotion(), 0});
        palette.push_back ({"go_to_xy","go to x: %n y: %n",cMotion(), 0});
        palette.push_back ({"go_to_random","go to random position",cMotion(), 0});
        palette.push_back ({"go_to_mouse","go to mouse-pointer",cMotion(), 0});
        palette.push_back ({"change_x","change x by %n",cMotion(), 0});
        palette.push_back ({"change_y","change y by %n",cMotion(), 0});
        palette.push_back ({"point_dir","point in direction %n",cMotion(), 0});

        palette.push_back ({"say_text","say %s",cLooks(), 1});
        palette.push_back ({"say_for_sec","say %s for %n secs",cLooks(), 1});
        palette.push_back ({"think_text","think %s",cLooks(), 1});
        palette.push_back ({"think_for_sec","think %s for %n secs",cLooks(), 1});
        palette.push_back ({"switch_costume","switch costume to %s",cLooks(), 1});
        palette.push_back ({"next_costume","next costume",cLooks(), 1});
        palette.push_back ({"switch_backdrop","switch backdrop to %s",cLooks(), 1});
        palette.push_back ({"next_backdrop","next backdrop",cLooks(), 1});
        palette.push_back ({"change_size","change size by %n",cLooks(), 1});
        palette.push_back ({"set_size","set size to %n %",cLooks(), 1});
        palette.push_back ({"show_sprite","show",cLooks(), 1});
        palette.push_back ({"hide_sprite","hide",cLooks(), 1});
        palette.push_back ({"go_to_layer","go to %s layer",cLooks(), 1});
        palette.push_back ({"go_layers","go %s %n layers",cLooks(), 1});

        palette.push_back ({"start_sound","start sound",cSound(), 2});
        palette.push_back ({"play_until_done","play sound until done",cSound(), 2});
        palette.push_back ({"stop_sound","stop all sounds",cSound(), 2});
        palette.push_back ({"set_volume","set volume to %n %",cSound(), 2});
        palette.push_back ({"change_volume","change volume by %n",cSound(), 2});

        palette.push_back ({"when_flag","when green flag clicked",cEvents(), 3});
        palette.push_back ({"when_key","when %s key pressed",cEvents(), 3});
        palette.push_back ({"when_clicked","when this sprite clicked",cEvents(), 3});
        palette.push_back ({"when_backdrop","when backdrop switches to %s",cEvents(), 3});
        palette.push_back ({"when_receive","when I receive %s",cEvents(), 3});
        palette.push_back ({"broadcast","broadcast %s",cEvents(), 3});
        palette.push_back ({"broadcast_wait","broadcast %s and wait",cEvents(), 3});

        palette.push_back ({"wait_sec","wait %n seconds",cControl(), 4});
        palette.push_back ({"repeat","repeat %n",cControl(), 4});
        palette.push_back ({"forever","forever",cControl(), 4});
        palette.push_back ({"if_block","if %b then",cControl(), 4});
        palette.push_back ({"if_else","if %b then else",cControl(), 4});
        palette.push_back ({"wait_until","wait until %b",cControl(), 4});
        palette.push_back ({"repeat_until","repeat until %b",cControl(), 4});
        palette.push_back ({"stop_all","stop all",cControl(), 4});
        palette.push_back ({"stop_script","stop this script",cControl(), 4});

        palette.push_back ({"touching_sprite","touching %s ?",cSensing(), 5});
        palette.push_back ({"touching_edge","touching edge?",cSensing(), 5});
        palette.push_back ({"distance_to","distance to %s",cSensing(), 5});
        palette.push_back ({"mouse_x","mouse x",cSensing(), 5});
        palette.push_back ({"mouse_y","mouse y",cSensing(), 5});

        palette.push_back ({"op_add","%n + %n",cOperator(), 6});
        palette.push_back ({"op_sub","%n - %n",cOperator(), 6});
        palette.push_back ({"op_mul","%n * %n",cOperator(), 6});
        palette.push_back ({"op_div","%n / %n",cOperator(), 6});
        palette.push_back ({"op_mod","%n mod %n",cOperator(), 6});
        palette.push_back ({"op_random","pick random %n to %n",cOperator(), 6});
        palette.push_back ({"op_greater","%n > %n",cOperator(), 6});
        palette.push_back ({"op_less","%n < %n",cOperator(), 6});
        palette.push_back ({"op_equal","%n = %n",cOperator(), 6});
        palette.push_back ({"op_and","%b and %b",cOperator(), 6});
        palette.push_back ({"op_or","%b or %b",cOperator(), 6});
        palette.push_back ({"op_not","not %b",cOperator(), 6});
        palette.push_back ({"op_join","join %s %s",cOperator(), 6});
        palette.push_back ({"op_letter_of","letter %n of %s",cOperator(), 6});
        palette.push_back ({"op_length","length of %s",cOperator(), 6});
        palette.push_back ({"op_contains","%s contains %s ?", cOperator(), 6});
        palette.push_back ({"op_round","round %n", cOperator(), 6});
        palette.push_back ({"op_abs","abs %n", cOperator(), 6});
        palette.push_back ({"op_floor","floor %n", cOperator(), 6});
        palette.push_back ({"op_ceil","ceil %n", cOperator(), 6});
        palette.push_back ({"op_sqrt","sqrt %n", cOperator(), 6});
        palette.push_back ({"op_sin", "sin %n", cOperator(), 6});
        palette.push_back ({"op_cos", "cos %n", cOperator(), 6});
        palette.push_back ({"op_tan", "tan %n", cOperator(), 6});

        palette.push_back ({"set_var", "set %s to %n",cVariable(), 7});
        palette.push_back ({"change_var", "change %s by %n",cVariable(), 7});
        palette.push_back ({"show_var", "show variable %s",cVariable(), 7});
        palette.push_back ({"hide_var", "hide variable %s", cVariable(), 7});

        palette.push_back ({"pen_erase_all", "erase all", cPen(), 8});
        palette.push_back ({"pen_stamp", "stamp", cPen(), 8});
        palette.push_back ({"pen_down", "pen down", cPen(), 8});
        palette.push_back ({"pen_up", "pen up", cPen(), 8});
        palette.push_back ({"pen_set_color","set pen color to %s", cPen(), 8});
        palette.push_back ({"pen_set_width","set pen size to %n", cPen(), 8});
        palette.push_back ({"pen_change_width","change pen size by %n", cPen(), 8});
    }
    bool shouldShowPaletteBlock (int paletteIdx) const
    {
        if (paletteIdx < 0 || paletteIdx >= (int)palette.size ()) return false;
        if (palette[paletteIdx].category == 8 && !showPenBlocks) return false;
        return true;
    }

    int layoutBlockWidth (BlockInst &b, TTF_Font *font)
    {
        if (b.paletteIdx < 0 || b.paletteIdx >= (int)palette.size ()) return 100;
        const string &lbl = palette[b.paletteIdx].label;
        int totalW = 12; // left padding
        int fi = 0;
        b.fieldRects.clear ();
        int i = 0;
        while (i < (int)lbl.size ())
        {
            if (i + 1 < (int)lbl.size () && lbl[i] == '%' &&
                (lbl[i+1]=='n'||lbl[i+1]=='s'||lbl[i+1]=='b'))
            {
                int fw = 40;
                if (fi < (int)b.args.size ())
                {
                    if (b.args[fi].kind == ArgVal::BOOL)
                        fw = 24;
                    else
                    {
                        const string &txt = b.args[fi].sVal;
                        if (font && !txt.empty ())
                        {
                            int tw = 0, th = 0;
                            TTF_SizeText (font, txt.c_str (), &tw, &th);
                            fw = max (40, tw + 12);
                        }
                    }
                }
                SDL_Rect fr = {b.x + totalW, b.y + 4, fw, b.h - 8};
                b.fieldRects.push_back (fr);
                totalW += fw + 4;
                fi++;
                i += 2;
            }
            else
            {
                int segStart = i;
                while (i < (int)lbl.size () && !(i + 1 < (int)lbl.size () && lbl[i]=='%' &&
                       (lbl[i+1]=='n'||lbl[i+1]=='s'||lbl[i+1]=='b')))
                    i++;
                string seg = lbl.substr (segStart, i - segStart);
                if (font && !seg.empty ())
                {
                    int tw = 0, th = 0;
                    TTF_SizeText (font, seg.c_str (), &tw, &th);
                    totalW += tw + 4;
                }
                else
                    totalW += (int)seg.size () * 8;
            }
        }
        totalW += 8;
        b.w = max (totalW, 60);
        return b.w;
    }

    void layoutBlock (BlockInst &b, TTF_Font *font)
    {
        layoutBlockWidth (b, font);
        int fi = 0;
        int px = b.x + 12;
        const string &lbl = palette[b.paletteIdx].label;
        int i = 0;
        while (i < (int)lbl.size ())
        {
            if (i + 1 < (int)lbl.size () && lbl[i] == '%' &&
                (lbl[i+1]=='n'||lbl[i+1]=='s'||lbl[i+1]=='b'))
            {
                if (fi < (int)b.fieldRects.size ())
                {
                    b.fieldRects[fi].x = px;
                    b.fieldRects[fi].y = b.y + 4;
                    px += b.fieldRects[fi].w + 4;
                }
                fi++;
                i += 2;
            }
            else
            {
                int segStart = i;
                while (i < (int)lbl.size () && !(i + 1 < (int)lbl.size () && lbl[i]=='%' &&
                       (lbl[i+1]=='n'||lbl[i+1]=='s'||lbl[i+1]=='b')))
                    i++;
                string seg = lbl.substr (segStart, i - segStart);
                if (font && !seg.empty ())
                {
                    int tw = 0, th = 0;
                    TTF_SizeText (font, seg.c_str (), &tw, &th);
                    px += tw + 4;
                }
                else
                    px += (int)seg.size () * 8;
            }
        }
    }

    struct CatButton { string name; SDL_Color color; int cat; SDL_Rect rect; };

    vector<CatButton> catButtons ()
    {
        vector<CatButton> v;
        int bx = paletteRect.x + 2, by = paletteRect.y + 4;
        int bw = paletteRect.w - 4, bh = 22;
        auto add = [&](string n, SDL_Color c, int cat)
        {
            v.push_back ({n, c, cat, {bx, by, bw, bh}});
            by += bh + 2;
        };
        add ("Motion",cMotion(),0);
        add ("Looks",cLooks(),1);
        add ("Sound",cSound(),2);
        add ("Events",cEvents(),3);
        add ("Control",cControl(),4);
        add ("Sensing",cSensing(),5);
        add ("Operators",cOperator(), 6);
        add ("Variables",cVariable(), 7);
        if (showPenBlocks)
        {
            add ("Pen",cPen(),8);
        }
        return v;
    }


    int catBarHeight ()
    {
        int count = 8;
        if (showPenBlocks) count = 9;
        return 24 * count + 2 * count + 8;
    }

    void renderText (SDL_Renderer *r, TTF_Font *f, const string &txt,
                     int x, int y, SDL_Color c = {255,255,255,255})
    {
        if (!f || txt.empty ()) return;
        SDL_Surface *s = TTF_RenderText_Blended (f, txt.c_str (), c);
        if (!s) return;
        SDL_Texture *t = SDL_CreateTextureFromSurface (r, s);
        SDL_Rect rc = {x, y, s->w, s->h};
        SDL_RenderCopy (r, t, nullptr, &rc);
        SDL_DestroyTexture (t);
        SDL_FreeSurface (s);
    }

    void renderOneBlock (SDL_Renderer *r, TTF_Font *f, BlockInst &b, bool ghost = false)
    {
        if (b.paletteIdx < 0) return;
        const PaletteBlock &pb = palette[b.paletteIdx];
        SDL_Color col = pb.color;
        if (ghost) { col.a = 128; }
        SDL_Rect body = {b.x, b.y, b.w, b.h};
        roundedBoxRGBA (r, body.x, body.y, body.x + body.w, body.y + body.h,
                        6, col.r, col.g, col.b, col.a);
        roundedRectangleRGBA (r, body.x, body.y, body.x + body.w, body.y + body.h,
                              6, 0, 0, 0, 60);
        const string &lbl = pb.label;
        int px = b.x + 12;
        int fi = 0;
        int ci = 0;
        while (ci < (int)lbl.size ())
        {
            if (ci + 1 < (int)lbl.size () && lbl[ci] == '%' &&
                (lbl[ci+1]=='n'||lbl[ci+1]=='s'||lbl[ci+1]=='b'))
            {
                if (fi < (int)b.fieldRects.size () && fi < (int)b.args.size ())
                {
                    SDL_Rect &fr = b.fieldRects[fi];
                    if (b.args[fi].kind == ArgVal::BOOL)
                    {
                        int cx = fr.x + fr.w / 2, cy = fr.y + fr.h / 2;
                        int rx = fr.w / 2, ry = fr.h / 2;
                        Sint16 vx[4] = {(Sint16)(cx - rx), (Sint16)cx, (Sint16)(cx + rx), (Sint16)cx};
                        Sint16 vy[4] = {(Sint16)cy, (Sint16)(cy - ry), (Sint16)cy, (Sint16)(cy + ry)};
                        filledPolygonRGBA (r, vx, vy, 4,
                                           b.args[fi].bVal ? 100 : 40,
                                           b.args[fi].bVal ? 200 : 40,
                                           b.args[fi].bVal ? 100 : 40, 255);
                        aapolygonRGBA (r, vx, vy, 4, 0, 0, 0, 100);
                    }
                    else
                    {
                        roundedBoxRGBA (r, fr.x, fr.y, fr.x + fr.w, fr.y + fr.h,
                                        4, 255, 255, 255, 220);
                        roundedRectangleRGBA (r, fr.x, fr.y, fr.x + fr.w, fr.y + fr.h,
                                              4, 0, 0, 0, 80);

                        string displayTxt = b.args[fi].sVal;
                        if (editActive && editBlockIdx >= 0 &&
                            editBlockIdx < (int)blocks.size () &&
                            &blocks[editBlockIdx] == &b && editFieldIdx == fi)
                        {
                            displayTxt = editBuf + "|";
                        }
                        if (!displayTxt.empty ())
                            renderText (r, f, displayTxt, fr.x + 4, fr.y + 2, {0,0,0,255});
                    }
                    px = fr.x + fr.w + 4;
                }
                fi++;
                ci += 2;
            }
            else
            {
                int segStart = ci;
                while (ci < (int)lbl.size () && !(ci + 1 < (int)lbl.size () && lbl[ci]=='%' &&
                       (lbl[ci+1]=='n'||lbl[ci+1]=='s'||lbl[ci+1]=='b')))
                    ci++;
                string seg = lbl.substr (segStart, ci - segStart);
                renderText (r, f, seg, px, b.y + 6, {255,255,255,255});
                if (f && !seg.empty ())
                {
                    int tw = 0, th = 0;
                    TTF_SizeText (f, seg.c_str (), &tw, &th);
                    px += tw + 4;
                }
                else
                    px += (int)seg.size () * 8;
            }
        }
    }

    void render (SDL_Renderer *r, TTF_Font *f)
    {
        SDL_SetRenderDrawColor (r, 230, 230, 230, 255);
        SDL_RenderFillRect (r, &paletteRect);
        auto cats = catButtons ();
        for (auto &cb : cats)
        {
            bool sel = (curCategory == cb.cat);
            Uint8 alpha = sel ? 255 : 180;
            roundedBoxRGBA (r, cb.rect.x, cb.rect.y,
                            cb.rect.x + cb.rect.w, cb.rect.y + cb.rect.h,
                            4, cb.color.r, cb.color.g, cb.color.b, alpha);
            renderText (r, f, cb.name, cb.rect.x + 6, cb.rect.y + 3, {255,255,255,255});
        }
        int palY = paletteRect.y + catBarHeight ();
        int palX = paletteRect.x + 6;
        for (int i = 0; i < (int)palette.size (); i++)
        {
            if (curCategory >= 0 && palette[i].category != curCategory) continue;
            if (!shouldShowPaletteBlock (i)) continue;
            BlockInst tmp;
            tmp.paletteIdx = i;
            tmp.args = palette[i].defaultArgs ();
            tmp.x = palX;
            tmp.y = palY - palScroll;
            tmp.h = 30;
            layoutBlock (tmp, f);
            if (tmp.y + tmp.h >= paletteRect.y && tmp.y <= paletteRect.y + paletteRect.h)
                renderOneBlock (r, f, tmp);
            palY += 36;
        }
        SDL_SetRenderDrawColor (r, 245, 245, 245, 255);
        SDL_RenderFillRect (r, &scriptRect);
        for (int i = 0; i < (int)blocks.size (); i++)
        {
            if (i == dragIdx && !draggingFromPalette) continue;
            layoutBlock (blocks[i], f);
            renderOneBlock (r, f, blocks[i]);
        }
        SDL_SetRenderDrawColor (r, 100, 100, 100, 60);
        for (auto &ch : chains)
        {
            for (int ci = 0; ci + 1 < (int)ch.blockIds.size (); ci++)
            {
                int a = ch.blockIds[ci], b = ch.blockIds[ci+1];
                if (a < 0 || b < 0 || a >= (int)blocks.size () || b >= (int)blocks.size ()) continue;
                int mx = blocks[a].x + 10;
                SDL_RenderDrawLine (r, mx, blocks[a].y + blocks[a].h,
                                       mx, blocks[b].y);
            }
        }
        if (dragIdx >= 0 && dragIdx < (int)blocks.size ())
        {
            layoutBlock (blocks[dragIdx], f);
            renderOneBlock (r, f, blocks[dragIdx], true);
        }
        if (globalRunning)
        {
            SDL_SetRenderDrawColor (r, 0, 200, 0, 100);
            SDL_Rect ind = {scriptRect.x, scriptRect.y, 8, 8};
            SDL_RenderFillRect (r, &ind);
        }
    }

    bool pointInRect (int px, int py, SDL_Rect &r)
    {
        return px >= r.x && px <= r.x + r.w && py >= r.y && py <= r.y + r.h;
    }
    int paletteHitTest (int mx, int my, TTF_Font *f)
    {
        int palY = paletteRect.y + catBarHeight ();
        int palX = paletteRect.x + 6;
        for (int i = 0; i < (int)palette.size (); i++)
        {
            if (curCategory >= 0 && palette[i].category != curCategory) continue;
            if (!shouldShowPaletteBlock (i)) continue;
            BlockInst tmp;
            tmp.paletteIdx = i;
            tmp.args = palette[i].defaultArgs ();
            tmp.x = palX;
            tmp.y = palY - palScroll;
            tmp.h = 30;
            layoutBlockWidth (tmp, f);
            SDL_Rect r = {tmp.x, tmp.y, tmp.w, tmp.h};
            if (pointInRect (mx, my, r)) return i;
            palY += 36;
        }
        return -1;
    }
    int blockHitTest (int mx, int my)
    {
        for (int i = (int)blocks.size () - 1; i >= 0; i--)
        {
            SDL_Rect r = {blocks[i].x, blocks[i].y, blocks[i].w, blocks[i].h};
            if (pointInRect (mx, my, r)) return i;
        }
        return -1;
    }
    int fieldHitTest (int blockIdx, int mx, int my)
    {
        if (blockIdx < 0 || blockIdx >= (int)blocks.size ()) return -1;
        for (int fi = 0; fi < (int)blocks[blockIdx].fieldRects.size (); fi++)
        {
            if (pointInRect (mx, my, blocks[blockIdx].fieldRects[fi]))
                return fi;
        }
        return -1;
    }

    void commitEdit ()
    {
        if (editActive && editBlockIdx >= 0 && editBlockIdx < (int)blocks.size ()
            && editFieldIdx >= 0 && editFieldIdx < (int)blocks[editBlockIdx].args.size ())
        {
            blocks[editBlockIdx].args[editFieldIdx].sVal = editBuf;
        }
        editActive = false;
        editBlockIdx = -1;
        editFieldIdx = -1;
        editBuf.clear ();
    }

    void startEdit (int bi, int fi)
    {
        commitEdit ();
        if (bi < 0 || fi < 0) return;
        if (blocks[bi].args[fi].kind == ArgVal::BOOL)
        {
            blocks[bi].args[fi].bVal = !blocks[bi].args[fi].bVal;
            return;
        }
        editBlockIdx = bi;
        editFieldIdx = fi;
        editBuf = blocks[bi].args[fi].sVal;
        editActive = true;
        SDL_StartTextInput ();
    }

    void handleEvent (SDL_Event &e, TTF_Font *f)
    {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            int mx = e.button.x, my = e.button.y;

            auto cats = catButtons ();
            for (auto &cb : cats)
            {
                if (pointInRect (mx, my, cb.rect))
                {
                    curCategory = (curCategory == cb.cat) ? -1 : cb.cat;
                    commitEdit ();
                    return;
                }
            }

            if (pointInRect (mx, my, paletteRect))
            {
                int pi = paletteHitTest (mx, my, f);
                if (pi >= 0)
                {
                    commitEdit ();
                    BlockInst nb;
                    nb.paletteIdx = pi;
                    nb.args = palette[pi].defaultArgs ();
                    nb.x = mx;
                    nb.y = my;
                    nb.h = 30;
                    layoutBlock (nb, f);
                    blocks.push_back (nb);
                    dragIdx = (int)blocks.size () - 1;
                    dragOffX = 0;
                    dragOffY = 0;
                    draggingFromPalette = true;
                }
                return;
            }

            if (pointInRect (mx, my, scriptRect))
            {
                int bi = blockHitTest (mx, my);
                if (bi >= 0)
                {
                    int fi = fieldHitTest (bi, mx, my);
                    if (fi >= 0)
                    {
                        startEdit (bi, fi);
                        return;
                    }
                    commitEdit ();
                    dragIdx = bi;
                    dragOffX = mx - blocks[bi].x;
                    dragOffY = my - blocks[bi].y;
                    draggingFromPalette = false;

                    for (auto &ch : chains)
                    {
                        auto it = find (ch.blockIds.begin (), ch.blockIds.end (), bi);
                        if (it != ch.blockIds.end ())
                        {
                            ch.blockIds.erase (it);
                            break;
                        }
                    }
                }
                else
                {
                    commitEdit ();
                }
                return;
            }

            commitEdit ();
        }

        if (e.type == SDL_MOUSEMOTION && dragIdx >= 0)
        {
            int mx = e.motion.x, my = e.motion.y;
            blocks[dragIdx].x = mx - dragOffX;
            blocks[dragIdx].y = my - dragOffY;
        }

        if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT && dragIdx >= 0)
        {
            int mx = e.button.x, my = e.button.y;

            if (!pointInRect (mx, my, scriptRect))
            {
                blocks.erase (blocks.begin () + dragIdx);
                for (auto &ch : chains)
                    for (auto &id : ch.blockIds)
                        if (id > dragIdx) id--;
            }
            dragIdx = -1;
            draggingFromPalette = false;
        }

        if (e.type == SDL_MOUSEWHEEL)
        {
            int mx, my;
            SDL_GetMouseState (&mx, &my);
            if (pointInRect (mx, my, paletteRect))
            {
                palScroll -= e.wheel.y * 20;
                if (palScroll < 0) palScroll = 0;
            }
        }
        if (editActive)
        {
            if (e.type == SDL_TEXTINPUT)
            {
                editBuf += e.text.text;
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_BACKSPACE && !editBuf.empty ())
                    editBuf.pop_back ();
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)
                    commitEdit ();
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    editBuf.clear ();
                    editActive = false;
                    editBlockIdx = -1;
                    editFieldIdx = -1;
                }
                if (e.key.keysym.sym == SDLK_TAB)
                {
                    /* move to next field */
                    commitEdit (); // save current
                    if (editBlockIdx >= 0 && editBlockIdx < (int)blocks.size ())
                    {
                        int nf = editFieldIdx + 1;
                        if (nf < (int)blocks[editBlockIdx].args.size ())
                            startEdit (editBlockIdx, nf);
                    }
                }
            }
        }
    }

    void autoRebuildChains ()
    {
        chains.clear ();
        vector<bool> used (blocks.size (), false);
        vector<int> order (blocks.size ());
        iota (order.begin (), order.end (), 0);
        sort (order.begin (), order.end (), [&](int a, int b)
        {
            if (blocks[a].y != blocks[b].y) return blocks[a].y < blocks[b].y;
            return blocks[a].x < blocks[b].x;
        });

        for (int idx : order)
        {
            if (used[idx]) continue;
            bool snapped = false;
            for (auto &ch : chains)
            {
                if (ch.blockIds.empty ()) continue;
                int last = ch.blockIds.back ();
                int bx = blocks[last].x;
                int by = blocks[last].y + blocks[last].h;

                if (abs (blocks[idx].x - bx) < 30 && abs (blocks[idx].y - by) < 15)
                {
                    blocks[idx].x = bx;
                    blocks[idx].y = by + 2;
                    ch.blockIds.push_back (idx);
                    used[idx] = true;
                    snapped = true;
                    break;
                }
            }
            if (!snapped)
            {
                ScriptChain sc;
                sc.blockIds.push_back (idx);
                chains.push_back (sc);
                used[idx] = true;
            }
        }
    }

    void startGreenFlag ()
    {
        autoRebuildChains ();
        execStates.clear ();
        globalRunning = true;

        for (int ci = 0; ci < (int)chains.size (); ci++)
        {
            if (chains[ci].blockIds.empty ()) continue;
            int first = chains[ci].blockIds[0];
            if (first < 0 || first >= (int)blocks.size ()) continue;
            int pi = blocks[first].paletteIdx;
            if (pi >= 0 && palette[pi].opcode == "when_flag")
            {
                ExecState es;
                es.chainIdx = ci;
                es.pc = 1; // skip the hat
                es.running = true;
                execStates.push_back (es);
            }
        }
    }

    void stopAll ()
    {
        globalRunning = false;
        execStates.clear ();
    }

    void execOneStep (ExecState &es,
                      Sprite &sprite,
                      Stage &stage,
                      vector<Sprite> &allSprites,
                      Pen &pen,
                      SDL_Texture *penLayer,
                      VarManager &vars,
                      MessageBus &msgBus,
                      TTF_Font *font,
                      SDL_Renderer *renderer,
                      bool &quit)
    {
        if (!es.running || es.stopped) return;
        if (es.chainIdx < 0 || es.chainIdx >= (int)chains.size ()) { es.running = false; return; }
        auto &ch = chains[es.chainIdx];
        if (es.pc < 0 || es.pc >= (int)ch.blockIds.size ()) { es.running = false; return; }

        if (es.waitUntilTick > 0)
        {
            if (SDL_GetTicks () < es.waitUntilTick) return;
            es.waitUntilTick = 0;
        }

        int bi = ch.blockIds[es.pc];
        if (bi < 0 || bi >= (int)blocks.size ()) { es.running = false; return; }
        BlockInst &blk = blocks[bi];
        int pi = blk.paletteIdx;
        if (pi < 0) { es.pc++; return; }
        const string &op = palette[pi].opcode;

        auto argN = [&](int i) -> double
        {
            if (i < (int)blk.args.size ()) return blk.args[i].asNum ();
            return 0;
        };
        auto argI = [&](int i) -> int
        {
            return (int)argN (i);
        };
        auto argS = [&](int i) -> string
        {
            if (i < (int)blk.args.size ()) return blk.args[i].sVal;
            return "";
        };
        auto argB = [&](int i) -> bool
        {
            if (i < (int)blk.args.size ()) return blk.args[i].bVal;
            return false;
        };

        if      (op == "move_steps")moveSteps (sprite, argI (0));
        else if (op == "turn_right")turnRight (sprite, argI (0));
        else if (op == "turn_left")turnLeft (sprite, argI (0));
        else if (op == "go_to_xy")goToCoords (sprite, false, false, argI (0), argI (1));
        else if (op == "go_to_random")goToCoords (sprite, false, true);
        else if (op == "go_to_mouse")goToCoords (sprite, true);
        else if (op == "change_x")changeCoordsBy (sprite, argI (0), 0);
        else if (op == "change_y")changeCoordsBy (sprite, 0, argI (0));
        else if (op == "point_dir")pointInDirection (sprite, argI (0));

        else if (op == "say_text") sayText (sprite, argS (0), font, renderer);
        else if (op == "say_for_sec") sayTextForSec (sprite, argS (0), argI (1), font, renderer);
        else if (op == "think_text") thinkText (sprite, argS (0), font, renderer);
        else if (op == "think_for_sec") thinkTextForSec (sprite, argS (0), argI (1), font, renderer);
        else if (op == "switch_costume") switchCostume (sprite, argS (0));
        else if (op == "next_costume") nextCostume (sprite);
        else if (op == "switch_backdrop") switchBackdrop (stage, argS (0));
        else if (op == "next_backdrop") nextBackdrop (stage);
        else if (op == "change_size") changeSize (sprite, argN (0));
        else if (op == "set_size") setSize (sprite, argN (0));
        else if (op == "show_sprite") showSprite (sprite);
        else if (op == "hide_sprite") hideSprite (sprite);
        else if (op == "go_to_layer") goToLayer (allSprites, sprite, argS (0), (int)allSprites.size ());
        else if (op == "go_layers") goLayers (allSprites, sprite, argS (0), argI (1), (int)allSprites.size ());

        else if (op == "start_sound") startSound (sprite);
        else if (op == "play_until_done") playSoundUntilDone (sprite);
        else if (op == "stop_sound") StopSound (sprite);
        else if (op == "set_volume") setSoundVolume (sprite, argN (0));
        else if (op == "change_volume") changeSoundVolume (sprite, argN (0));

        else if (op == "pen_erase_all") pen.eraseAll (renderer, penLayer);
        else if (op == "pen_stamp") stamp (renderer, sprite, penLayer);
        else if (op == "pen_down") { pen.penDown = true; pen.draw(renderer, sprite, penLayer); }
        else if (op == "pen_up") pen.penUp ();
        else if (op == "pen_set_color")
        {
            string cs = argS (0);
            SDL_Color c = {0, 0, 255, 255};
            if (cs.size () >= 7 && cs[0] == '#')
            {
                unsigned int hex = 0;
                sscanf (cs.c_str () + 1, "%x", &hex);
                c.r = (hex >> 16) & 0xFF;
                c.g = (hex >> 8) & 0xFF;
                c.b = hex & 0xFF;
            }
            else
            {
                int r2=0, g2=0, b2=0;
                if (sscanf (cs.c_str (), "%d,%d,%d", &r2, &g2, &b2) == 3)
                { c.r = r2; c.g = g2; c.b = b2; }
            }
            pen.setPenColor (c);
        }
        else if (op == "pen_set_width") pen.setPenWidth (argI (0));
        else if (op == "pen_change_width") pen.changePenWidth (argI (0));

        else if (op == "broadcast") msgBus.broadcast (argS (0));
        else if (op == "broadcast_wait") msgBus.broadcast (argS (0));

        else if (op == "wait_sec")
        {
            double secs = argN (0);
            es.waitUntilTick = SDL_GetTicks () + (Uint32)(secs * 1000);
        }
        else if (op == "repeat")
        {
            if (es.repeatPC != es.pc)
            {
                es.repeatCounter = argI (0);
                es.repeatPC = es.pc;
            }
            if (es.repeatCounter <= 0)
            {
                es.repeatPC = -1;
            }
            else
            {
                es.repeatCounter--;
                if (es.pc + 1 < (int)ch.blockIds.size ())
                {
                    es.pc++;
                    return;
                }
            }
        }
        else if (op == "forever")
        {
            es.foreverPC = es.pc;
        }
        else if (op == "if_block")
        {
            if (!argB (0))
            {
                es.pc++;
            }
        }
        else if (op == "if_else")
        {
            if (!argB (0))
            {
                es.pc++;
            }
        }
        else if (op == "wait_until")
        {
            if (!argB (0))
                return;
        }
        else if (op == "repeat_until")
        {
            if (!argB (0))
            {
                if (es.pc + 1 < (int)ch.blockIds.size ())
                {
                    es.pc++;
                    return;
                }
            }
        }
        else if (op == "stop_all") { quit = true; stopAll (); return; }
        else if (op == "stop_script")  { es.running = false; return; }

        else if (op == "set_var") vars.setVar (argS (0), argN (1));
        else if (op == "change_var") vars.changeVar (argS (0), argN (1));
        else if (op == "show_var") vars.showVar (argS (0));
        else if (op == "hide_var") vars.hideVar (argS (0));

        es.pc++;

        if (es.foreverPC >= 0 && es.pc >= (int)ch.blockIds.size ())
            es.pc = es.foreverPC + 1;

        if (es.repeatPC >= 0 && es.repeatCounter > 0 && es.pc > es.repeatPC)
        {
            es.pc = es.repeatPC;
        }

        if (es.pc >= (int)ch.blockIds.size ())
            es.running = false;
    }

    void updateAll (Sprite &sprite,
                    Stage &stage,
                    vector<Sprite> &allSprites,
                    Pen &pen,
                    SDL_Texture *penLayer,
                    VarManager &vars,
                    MessageBus &msgBus,
                    TTF_Font *font,
                    SDL_Renderer *renderer,
                    bool &quit)
    {
        if (!globalRunning) return;
        bool anyRunning = false;
        for (auto &es : execStates)
        {
            if (es.running && !es.stopped)
            {
                execOneStep (es, sprite, stage, allSprites, pen, penLayer,
                             vars, msgBus, font, renderer, quit);
                if (es.running) anyRunning = true;
            }
        }
        if (!anyRunning) globalRunning = false;
    }
};

#endif // CODING_ENGINE_H
