#ifndef BLOCK_SYSTEM_H
#define BLOCK_SYSTEM_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <vector>
#include <string>
#include <functional>

using namespace std;


enum struct BlockType {
    MOTION,
    LOOKS,
    SOUND,
    EVENTS,
    CONTROL,
    SENSING,
    OPERATORS,
    VARIABLES
};

enum struct BlockShape {
    STACK,
    HAT,
    CAP,
    REPORTER,
    BOOLEAN,
    C_BLOCK
};

struct BlockColors {
    SDL_Color motion    = {66, 133, 244, 255};
    SDL_Color looks     = {147, 88, 254, 255};
    SDL_Color sound     = {207, 99, 207, 255};
    SDL_Color events    = {255, 213, 0, 255};
    SDL_Color control   = {255, 171, 25, 255};
    SDL_Color sensing   = {90, 193, 242, 255};
    SDL_Color operators = {89, 192, 89, 255};
    SDL_Color variables = {255, 140, 26, 255};

    SDL_Color getColor(BlockType type) {
        switch(type) {
            case BlockType::MOTION:    return motion;
            case BlockType::LOOKS:     return looks;
            case BlockType::SOUND:     return sound;
            case BlockType::EVENTS:    return events;
            case BlockType::CONTROL:   return control;
            case BlockType::SENSING:   return sensing;
            case BlockType::OPERATORS: return operators;
            case BlockType::VARIABLES: return variables;
            default: return {128, 128, 128, 255};
        }
    }
};

struct BlockParameter {
    string name;
    string value;
    SDL_Rect inputRect;
    bool isEditing = false;

    BlockParameter(string n, string v) : name(n), value(v) {}
};


struct Block {
    int id;
    string name;
    string displayText;
    BlockType type;
    BlockShape shape;

    int x, y;
    int width, height;
    SDL_Rect rect;

    SDL_Color color;
    SDL_Color darkColor;

    vector<BlockParameter> parameters;

    Block* nextBlock = nullptr;
    Block* prevBlock = nullptr;
    Block* parentBlock = nullptr;
    vector<Block*> innerBlocks;

    bool isDragging = false;
    bool isInPalette = true;
    bool isHovered = false;

    int dragOffsetX, dragOffsetY;

    Block(int id, string name, string text, BlockType type, BlockShape shape = BlockShape::STACK)
            : id(id), name(name), displayText(text), type(type), shape(shape) {

        BlockColors colors;
        color = colors.getColor(type);

        darkColor = {
                (Uint8)(color.r * 0.7),
                (Uint8)(color.g * 0.7),
                (Uint8)(color.b * 0.7),
                255
        };

        width = 200;
        height = 40;

        if (shape == BlockShape::HAT) height = 50;
        if (shape == BlockShape::C_BLOCK) height = 80;
    }

    void addParameter(string name, string defaultValue) {
        parameters.push_back(BlockParameter(name, defaultValue));
    }

    void updateRect() {
        rect = {x, y, width, height};

        int paramX = x + 10;
        for (auto& param : parameters) {
            param.inputRect = {paramX + 80, y + 10, 50, 20};
            paramX += 140;
        }
    }

    void draw(SDL_Renderer* renderer, TTF_Font* font) {
        updateRect();

        switch(shape) {
            case BlockShape::STACK:
                drawStackBlock(renderer, font);
                break;
            case BlockShape::HAT:
                drawHatBlock(renderer, font);
                break;
            case BlockShape::REPORTER:
                drawReporterBlock(renderer, font);
                break;
            case BlockShape::BOOLEAN:
                drawBooleanBlock(renderer, font);
                break;
            case BlockShape::C_BLOCK:
                drawCBlock(renderer, font);
                break;
            default:
                drawStackBlock(renderer, font);
        }
    }

    void drawStackBlock(SDL_Renderer* renderer, TTF_Font* font) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
        SDL_Rect shadow = {x + 3, y + 3, width, height};
        SDL_RenderFillRect(renderer, &shadow);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect top = {x, y, width, 5};
        SDL_RenderFillRect(renderer, &top);
        if (prevBlock != nullptr || !isInPalette) {
            SDL_Rect notchTop = {x + 15, y - 4, 30, 4};
            SDL_RenderFillRect(renderer, &notchTop);
        }
        SDL_Rect body = {x, y + 5, width, height - 10};
        SDL_RenderFillRect(renderer, &body);
        SDL_Rect bottom = {x, y + height - 5, width, 5};
        SDL_RenderFillRect(renderer, &bottom);
        SDL_SetRenderDrawColor(renderer, darkColor.r, darkColor.g, darkColor.b, 255);
        SDL_Rect notchBottom = {x + 15, y + height, 30, 4};
        SDL_RenderFillRect(renderer, &notchBottom);
        if (isHovered) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
        drawText(renderer, font);
        drawParameters(renderer, font);
    }

    void drawHatBlock(SDL_Renderer* renderer, TTF_Font* font) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
        filledEllipseRGBA(renderer, x + width/2 + 3, y + 15 + 3, width/2, 15, 0, 0, 0, 50);
        filledEllipseRGBA(renderer, x + width/2, y + 15, width/2, 15,
                          color.r, color.g, color.b, color.a);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect body = {x, y + 15, width, height - 20};
        SDL_RenderFillRect(renderer, &body);
        SDL_Rect bottom = {x, y + height - 5, width, 5};
        SDL_RenderFillRect(renderer, &bottom);
        SDL_SetRenderDrawColor(renderer, darkColor.r, darkColor.g, darkColor.b, 255);
        SDL_Rect notch = {x + 15, y + height, 30, 4};
        SDL_RenderFillRect(renderer, &notch);
        drawText(renderer, font, 0, 5);
    }

    void drawReporterBlock(SDL_Renderer* renderer, TTF_Font* font) {
        int radius = height / 2;
        filledEllipseRGBA(renderer, x + radius, y + radius, radius, radius,
                          color.r, color.g, color.b, color.a);
        filledEllipseRGBA(renderer, x + width - radius, y + radius, radius, radius,
                          color.r, color.g, color.b, color.a);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect middle = {x + radius, y, width - 2*radius, height};
        SDL_RenderFillRect(renderer, &middle);
        drawText(renderer, font);
    }

    void drawBooleanBlock(SDL_Renderer* renderer, TTF_Font* font) {
        int points = 6;
        Sint16 vx[6] = {(Sint16)(x + 15), (Sint16)x, (Sint16)(x + 15),
                        (Sint16)(x + width - 15), (Sint16)(x + width), (Sint16)(x + width - 15)};
        Sint16 vy[6] = {(Sint16)y, (Sint16)(y + height/2), (Sint16)(y + height),
                        (Sint16)(y + height), (Sint16)(y + height/2), (Sint16)y};

        filledPolygonRGBA(renderer, vx, vy, points,
                          color.r, color.g, color.b, color.a);

        drawText(renderer, font);
    }

    void drawCBlock(SDL_Renderer* renderer, TTF_Font* font) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        SDL_Rect top = {x, y, width, 35};
        SDL_RenderFillRect(renderer, &top);

        if (prevBlock != nullptr) {
            SDL_Rect notchTop = {x + 15, y - 4, 30, 4};
            SDL_RenderFillRect(renderer, &notchTop);
        }

        SDL_Rect left = {x, y + 35, 20, height - 70};
        SDL_RenderFillRect(renderer, &left);

        SDL_Rect bottom = {x, y + height - 35, width, 35};
        SDL_RenderFillRect(renderer, &bottom);

        SDL_SetRenderDrawColor(renderer, darkColor.r, darkColor.g, darkColor.b, 255);
        SDL_Rect innerNotch = {x + 35, y + 35, 30, 4};
        SDL_RenderFillRect(renderer, &innerNotch);

        SDL_Rect notchBottom = {x + 15, y + height, 30, 4};
        SDL_RenderFillRect(renderer, &notchBottom);

        drawText(renderer, font, 0, -height/2 + 25);
    }

    void drawText(SDL_Renderer* renderer, TTF_Font* font, int offsetX = 0, int offsetY = 0) {
        if (!font) return;

        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font, displayText.c_str(), white);
        if (!surface) return;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect textRect = {
                x + 10 + offsetX,
                y + (height - surface->h) / 2 + offsetY,
                surface->w,
                surface->h
        };

        SDL_RenderCopy(renderer, texture, nullptr, &textRect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    void drawParameters(SDL_Renderer* renderer, TTF_Font* font) {
        if (!font) return;

        for (auto& param : parameters) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &param.inputRect);
            SDL_SetRenderDrawColor(renderer, darkColor.r, darkColor.g, darkColor.b, 255);
            SDL_RenderDrawRect(renderer, &param.inputRect);
            SDL_Color black = {0, 0, 0, 255};
            SDL_Surface* surface = TTF_RenderText_Blended(font, param.value.c_str(), black);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_Rect textRect = {
                        param.inputRect.x + 5,
                        param.inputRect.y + 2,
                        surface->w,
                        surface->h
                };
                SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
            }
        }
    }

    bool containsPoint(int px, int py) {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }

    void startDrag(int mouseX, int mouseY) {
        isDragging = true;
        dragOffsetX = mouseX - x;
        dragOffsetY = mouseY - y;
    }

    void updateDrag(int mouseX, int mouseY) {
        if (isDragging) {
            x = mouseX - dragOffsetX;
            y = mouseY - dragOffsetY;
            updateConnectedBlocks();
        }
    }

    void endDrag() {
        isDragging = false;
    }

    void updateConnectedBlocks() {
        if (nextBlock) {
            nextBlock->x = x;
            nextBlock->y = y + height + 4;
            nextBlock->updateConnectedBlocks();
        }
    }

    Block* clone() {
        Block* newBlock = new Block(id, name, displayText, type, shape);
        newBlock->width = width;
        newBlock->height = height;
        newBlock->parameters = parameters;
        newBlock->isInPalette = false;
        return newBlock;
    }
};

#endif
