#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "raylib.h"
#include "context.h"

struct Background
{
    float speed;
    Vector2 pos;
    int width, height;
    Texture2D tex;
    float currentPosMove;
    Context *context;
};
typedef struct Background Background;

Background* makeBackGround(const char* source, int speed);
void BackgroundDraw(Background *bg, float frameTime);

#endif // MACRO
