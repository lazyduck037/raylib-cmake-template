#ifndef BIRD_H
#define BIRD_H

#include "raylib.h"
#include <stdlib.h>

struct Bird {
    Vector2 location;
    float angle;
    float speed;
    Texture2D textureMid;
    Texture2D textureUp;
    Texture2D textureDown;
    Texture2D curTex;
    Rectangle rectangle;
    int state;

};
typedef struct Bird Bird;


Bird* makeBird(const char *upTex,const char *midTex, const char *dowTex);
void releaseBird(Bird*c);
void drawBird(Bird *c, float frameTime);
void inputControl(Bird *bird, float frameTime);

#endif
