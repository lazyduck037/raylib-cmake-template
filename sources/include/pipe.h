#ifndef PIPE_H
#define PIPE_H

#include "raylib.h"
#include <stdlib.h>

struct Pipe {
    Rectangle top;
    Rectangle bottom;
    bool passed;
    Texture2D topTex;
    Texture2D botTex;
};
typedef struct Pipe Pipe;

void makePipes(const char *pipe, int baseHeight, float speed);
void drawPipe(float frameTime);

#endif