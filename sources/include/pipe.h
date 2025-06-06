#ifndef PIPE_H
#define PIPE_H

#include "raylib.h"
#include <stdlib.h>
#include "bird.h"

struct Pipe {
    Rectangle desTop;
    Rectangle sourceTop;

    Rectangle desBot;
    Rectangle sourceBot;
    Context *contex;
};
typedef struct Pipe Pipe;

void makePipes(const char *pipe, int baseHeight, float speed, Pipe ***out, int *outNumber);
void drawPipe(Pipe **pipes,int numberPipe,float frameTime);
bool checkHitPipe(Pipe **pipes, int numPipes, Bird * bird);
void releasePipe(Pipe **pipes, int numberPipe);
#endif