#ifndef SCORE_H
#define SCORE_H

#include "context.h"
#include "raylib.h"
#include "resource.h"
#include "define.h"

struct Score {
    int x,y;
    Texture2D zero, one, two, three, four, five, six, seven, eight, nine;
    Context *contex;
    int score;
};
typedef struct Score Score;

Score* makeScore();
void drawScore(Score *score, int num);
void releaseScore(Score *score);
#endif