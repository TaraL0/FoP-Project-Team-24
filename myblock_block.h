#ifndef MYBLOCK_BLOCK_H
#define MYBLOCK_BLOCK_H

#include "sprite.h"

struct Param
{
    string name;
    string type;
    double numVal = 0;
    bool boolVal = false;
    string strVal;
};

struct CustomBlock
{
    string name;
    vector <Param> params;
    function <void (vector <Param> &)> body;

    void call (vector <Param> args)
    {
        body (args);
    }
};

struct BlockRegistry
{
    vector <CustomBlock> blocks;

    bool exists (string name)
    {
        for (auto &b : blocks)
            if (b.name == name) return true;
        return false;
    }

    bool define (string name, vector <Param> params, function <void (vector <Param> &)> body)
    {
        if (exists (name)) return false;
        CustomBlock cb;
        cb.name = name;
        cb.params = params;
        cb.body = body;
        blocks.push_back (cb);
        return true;
    }

    void callBlock (string name, vector <Param> args)
    {
        for (auto &b : blocks)
            if (b.name == name) { b.call (args); return; }
    }
};

struct CustomBlockDefResult
{
    bool confirmed = false;
    string blockName;
    vector <Param> params;
};

inline CustomBlockDefResult runCustomBlockMenu (SDL_Renderer *m_renderer, TTF_Font *font, BlockRegistry &reg)
{
    CustomBlockDefResult result;
    bool done = false;

    string blockName;
    vector <Param> params;
    string newParamName;
    string newParamType = "int";
    string errorMsg;

    int panelX = 400, panelY = 200, panelW = 700, panelH = 550;
    SDL_Rect panel = {panelX, panelY, panelW, panelH};

    SDL_Rect nameField    = {panelX + 20, panelY + 80,  panelW - 40, 40};
    SDL_Rect pNameField   = {panelX + 20, panelY + 220, 300, 40};
    SDL_Rect typeInt      = {panelX + 20, panelY + 280, 130, 36};
    SDL_Rect typeBool     = {panelX + 160, panelY + 280, 130, 36};
    SDL_Rect typeStr      = {panelX + 300, panelY + 280, 130, 36};
    SDL_Rect addParamBtn  = {panelX + 450, panelY + 215, 200, 44};
    SDL_Rect confirmBtn   = {panelX + 20,  panelY + panelH - 60, 200, 44};
    SDL_Rect cancelBtn    = {panelX + 240, panelY + panelH - 60, 200, 44};

    enum Focus { NONE, BLOCK_NAME, PARAM_NAME } focus = NONE;

    SDL_StartTextInput ();

    while (!done)
    {
        SDL_Event ev;
        while (SDL_PollEvent (&ev))
        {
            if (ev.type == SDL_KEYDOWN)
            {
                SDL_Keycode k = ev.key.keysym.sym;
                if (focus == BLOCK_NAME)
                {
                    if (k == SDLK_BACKSPACE and !blockName.empty ()) blockName.pop_back ();
                    if (k == SDLK_TAB) focus = PARAM_NAME;
                }
                else if (focus == PARAM_NAME)
                {
                    if (k == SDLK_BACKSPACE and !newParamName.empty ()) newParamName.pop_back ();
                    if (k == SDLK_TAB) focus = BLOCK_NAME;
                }
                if (k == SDLK_ESCAPE) { done = true; return result; }
            }
            if (ev.type == SDL_TEXTINPUT)
            {
                if (focus == BLOCK_NAME)  blockName  += ev.text.text;
                if (focus == PARAM_NAME)  newParamName += ev.text.text;
            }
            if (ev.type == SDL_MOUSEBUTTONDOWN and ev.button.button == SDL_BUTTON_LEFT)
            {
                int mx, my;
                SDL_GetMouseState (&mx, &my);

                auto hit = [&](SDL_Rect r){ return mx >= r.x and mx <= r.x + r.w and my >= r.y and my <= r.y + r.h; };

                focus = NONE;
                if (hit (nameField))   focus = BLOCK_NAME;
                if (hit (pNameField))  focus = PARAM_NAME;

                if (hit (typeInt))  { newParamType = "int"; }
                if (hit (typeBool)) { newParamType = "bool"; }
                if (hit (typeStr))  { newParamType = "string"; }

                if (hit (addParamBtn))
                {
                    errorMsg.clear ();
                    if (newParamName.empty ())
                        errorMsg = "Parameter name cannot be empty.";
                    else
                    {
                        bool dup = false;
                        for (auto &p : params)
                            if (p.name == newParamName) { dup = true; break; }
                        if (dup)
                            errorMsg = "Duplicate parameter name.";
                        else
                        {
                            Param p;
                            p.name = newParamName;
                            p.type = newParamType;
                            params.push_back (p);
                            newParamName.clear ();
                        }
                    }
                }

                if (hit (confirmBtn))
                {
                    errorMsg.clear ();
                    if (blockName.empty ())
                        errorMsg = "Block name cannot be empty.";
                    else if (reg.exists (blockName))
                        errorMsg = "A block named '" + blockName + "' already exists.";
                    else
                    {
                        result.confirmed = true;
                        result.blockName = blockName;
                        result.params    = params;
                        done = true;
                    }
                }

                if (hit (cancelBtn))
                    done = true;
            }
        }

        SDL_SetRenderDrawColor (m_renderer, 0, 0, 0, 120);
        SDL_Rect overlay = {0, 0, 1920, 1080};
        SDL_SetRenderDrawBlendMode (m_renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect (m_renderer, &overlay);

        SDL_SetRenderDrawColor (m_renderer, 245, 245, 245, 255);
        SDL_RenderFillRect (m_renderer, &panel);
        SDL_SetRenderDrawColor (m_renderer, 60, 60, 60, 255);
        SDL_RenderDrawRect (m_renderer, &panel);

        if (font)
        {
            SDL_Color black   = {0, 0, 0, 255};
            SDL_Color white   = {255, 255, 255, 255};
            SDL_Color accent  = {255, 100, 0, 255};
            SDL_Color gray    = {180, 180, 180, 255};
            SDL_Color red     = {200, 0, 0, 255};
            SDL_Color selCol  = {80, 80, 200, 255};

            auto renderText = [&](string txt, SDL_Color c, int x, int y)
            {
                SDL_Surface *s = TTF_RenderText_Blended (font, txt.c_str (), c);
                if (!s) return;
                SDL_Texture *t = SDL_CreateTextureFromSurface (m_renderer, s);
                SDL_Rect r = {x, y, s -> w, s -> h};
                SDL_RenderCopy (m_renderer, t, nullptr, &r);
                SDL_DestroyTexture (t);
                SDL_FreeSurface (s);
            };

            renderText ("Make a Block", accent, panelX + 20, panelY + 20);
            renderText ("Block name:", black, panelX + 20, panelY + 55);

            SDL_SetRenderDrawColor (m_renderer, focus == BLOCK_NAME ? 200 : 220,
                                                focus == BLOCK_NAME ? 220 : 220,
                                                focus == BLOCK_NAME ? 255 : 220, 255);
            SDL_RenderFillRect (m_renderer, &nameField);
            SDL_SetRenderDrawColor (m_renderer, focus == BLOCK_NAME ? 80 : 150,
                                                focus == BLOCK_NAME ? 80 : 150,
                                                focus == BLOCK_NAME ? 200 : 150, 255);
            SDL_RenderDrawRect (m_renderer, &nameField);
            string nameDisplay = blockName + (focus == BLOCK_NAME ? "|" : "");
            if (!nameDisplay.empty ())
                renderText (nameDisplay, black, nameField.x + 6, nameField.y + 8);

            renderText ("Add parameter:", black, panelX + 20, panelY + 185);
            renderText ("Name:", black, panelX + 20, panelY + 195);

            SDL_SetRenderDrawColor (m_renderer, focus == PARAM_NAME ? 200 : 220,
                                                focus == PARAM_NAME ? 220 : 220,
                                                focus == PARAM_NAME ? 255 : 220, 255);
            SDL_RenderFillRect (m_renderer, &pNameField);
            SDL_SetRenderDrawColor (m_renderer, focus == PARAM_NAME ? 80 : 150,
                                                focus == PARAM_NAME ? 80 : 150,
                                                focus == PARAM_NAME ? 200 : 150, 255);
            SDL_RenderDrawRect (m_renderer, &pNameField);
            string pDisplay = newParamName + (focus == PARAM_NAME ? "|" : "");
            if (!pDisplay.empty ())
                renderText (pDisplay, black, pNameField.x + 6, pNameField.y + 8);

            renderText ("Type:", black, panelX + 20, panelY + 260);

            auto drawTypeBtn = [&](SDL_Rect r, string label, bool selected)
            {
                SDL_SetRenderDrawColor (m_renderer, selected ? 80 : 210,
                                                    selected ? 80 : 210,
                                                    selected ? 200 : 210, 255);
                SDL_RenderFillRect (m_renderer, &r);
                SDL_SetRenderDrawColor (m_renderer, 60, 60, 60, 255);
                SDL_RenderDrawRect (m_renderer, &r);
                renderText (label, selected ? white : black, r.x + 8, r.y + 8);
            };

            drawTypeBtn (typeInt,  "int",    newParamType == "int");
            drawTypeBtn (typeBool, "bool",   newParamType == "bool");
            drawTypeBtn (typeStr,  "string", newParamType == "string");

            SDL_SetRenderDrawColor (m_renderer, 60, 180, 80, 255);
            SDL_RenderFillRect (m_renderer, &addParamBtn);
            SDL_SetRenderDrawColor (m_renderer, 30, 120, 50, 255);
            SDL_RenderDrawRect (m_renderer, &addParamBtn);
            renderText ("+ Add Parameter", white, addParamBtn.x + 10, addParamBtn.y + 10);

            renderText ("Parameters:", black, panelX + 20, panelY + 335);
            int py = panelY + 360;
            if (params.empty ())
                renderText ("(none)", gray, panelX + 20, py);
            else
                for (int i = 0; i < (int)params.size (); i++)
                {
                    string ptxt = to_string (i + 1) + ". " + params [i].name + "  [" + params [i].type + "]";
                    renderText (ptxt, selCol, panelX + 20, py);
                    py += 28;
                }

            if (!errorMsg.empty ())
                renderText (errorMsg, red, panelX + 20, panelY + panelH - 90);

            SDL_SetRenderDrawColor (m_renderer, 80, 80, 200, 255);
            SDL_RenderFillRect (m_renderer, &confirmBtn);
            SDL_SetRenderDrawColor (m_renderer, 40, 40, 140, 255);
            SDL_RenderDrawRect (m_renderer, &confirmBtn);
            renderText ("Confirm", white, confirmBtn.x + 50, confirmBtn.y + 10);

            SDL_SetRenderDrawColor (m_renderer, 180, 60, 60, 255);
            SDL_RenderFillRect (m_renderer, &cancelBtn);
            SDL_SetRenderDrawColor (m_renderer, 120, 30, 30, 255);
            SDL_RenderDrawRect (m_renderer, &cancelBtn);
            renderText ("Cancel", white, cancelBtn.x + 55, cancelBtn.y + 10);
        }

        SDL_RenderPresent (m_renderer);
    }

    SDL_StopTextInput ();
    return result;
}

#endif //MYBLOCK_BLOCK_H