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
    bool isPass;
};
typedef struct Pipe Pipe;

enum Collision {
  HitSky,
  HitGround,
  HitPipe,
  PassPipe,
  None
};

void makePipes(const char *pipe, int baseHeight, float speed, Pipe ***out, int *outNumber);
void drawPipe(Pipe **pipes,int numberPipe,float frameTime);
enum Collision checkHit(Pipe **pipes, int numPipes, Bird * bird);
void enablePipe();
void releasePipe(Pipe **pipes, int numberPipe);
#endif