#ifndef BIRD_H
#define BIRD_H

#include "raylib.h"
#include <stdlib.h>
#include "context.h"

struct Bird {
    float angle;
    float speedRotate;

    float speed;
    Texture2D textureMid;
    Texture2D textureUp;
    Texture2D textureDown;
    Texture2D curTex;
    Rectangle rectangle;
    Rectangle des;
    int state;
    Context *contex;
    bool isFall;

    //sound;
    Sound wing, hit,die, point;
};
typedef struct Bird Bird;

Bird* makeBird(const char *upTex,const char *midTex, const char *dowTex);
void releaseBird(Bird*c);
void drawBird(Bird *c, float frameTime);
int inputControl(Bird *bird, int baseY,float frameTime);
void jumbBird(Bird *bird);
void playFallBird(Bird *bird);
void playEarnPoint(Bird *bird);
#endif
