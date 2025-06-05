#ifndef PIPE_H
#define PIPE_H

#include "raylib.h"
#include <stdlib.h>

struct Pipe {
    Rectangle desTop;
    Rectangle sourceTop;

    Rectangle desBot;
    Rectangle sourceBot;
    bool passed;
    Texture2D topTex;
    Texture2D botTex;
};
typedef struct Pipe Pipe;

void makePipes(const char *pipe, int baseHeight, float speed, Pipe ***out, int *outNumber);
void drawPipe(Pipe **pipes,int numberPipe,float frameTime);
void releasePipe(Pipe **pipes, int numberPipe);
#endif