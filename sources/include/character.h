#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include <stdlib.h>

struct Character {
    Vector2 location;
    float angle;
    float speed;
    float currentSpeed;
    Vector2 oriLocation;
    float currentTime;
    Texture2D textureMid;
    Texture2D textureUp;
    Texture2D textureDown;
    Rectangle rectangle;
    int state;

};
typedef struct Character Character;


Character* makeCharacter(const char *upTex,const char *midTex, const char *dowTex);
void releaseCharacter(Character*c);
void drawCharacter(Character *c);
void inputControl(Character *character, float frameTime);
#endif
