#ifndef PIPE_H
#define PIPE_H

#include "raylib.h"
#include <stdlib.h>

struct Pipe {
    Rectangle top;
    Rectangle bottom;
    bool passed;
};
typedef struct Pipe Pipe;

void makePipes(const char *pipe);
void drawPipe();

#endif