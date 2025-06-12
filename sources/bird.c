#include "bird.h"
#include "define.h"
#include "context.h"
#include <math.h>    
#include <stdio.h>
#include "resource.h"

#define CHAR_STATE_UP 2
#define CHAR_STATE_MID 1
#define CHAR_STATE_DOWN 0

// #define JUMP_FORCE -17.0f
// #define JUMP_FORCE -15.0f
// #define GRAVITY 0.8f

#define ACCELERATION_ROTATE 0.1f

float mElapsedTime = 0.0f;
float mElapsedRotate = 0.0f;
bool isStart = false;
int frameAnim = 0;
float acceleration = 0.05;
bool lastDirection = false;
bool isPlaySoundDie = false;
bool isPlaySoundHit = false;

static void updateAnimation(Bird *bird) {
    if(bird->contex->state == Stop) {
        bird->curTex = bird->textureMid;
        return;
    }
    if(mElapsedTime >= 0.3f) {
        bird->state++;
        if(bird->state > 2) {
            bird->state = CHAR_STATE_DOWN;
        }

        switch (bird->state) {
            case CHAR_STATE_UP:
                bird->curTex = bird->textureUp;
                break;
            case CHAR_STATE_DOWN:
                bird->curTex = bird->textureDown;
                break;
            default:
                bird->curTex = bird->textureMid;
                break;
        }
        
        mElapsedTime = 0;
    }
}

Bird* makeBird(const char *upTex,const char *midTex, const char *dowTex) 
{
    Image birdImage = LoadImage(upTex);
    ImageResize(&birdImage, (int)(birdImage.width*1.5),(int)(birdImage.height*1.5));
    Texture2D birdUpTexture = LoadTextureFromImage(birdImage);
    UnloadImage(birdImage);

    birdImage = LoadImage(midTex);
    ImageResize(&birdImage, (int)(birdImage.width*1.5),(int)(birdImage.height*1.5));
    Texture2D birdMidTexture = LoadTextureFromImage(birdImage);
    UnloadImage(birdImage);

    birdImage = LoadImage(dowTex);
    ImageResize(&birdImage, (int)(birdImage.width*1.5),(int)(birdImage.height*1.5));
    Texture2D birdDownTexture = LoadTextureFromImage(birdImage);
    UnloadImage(birdImage);
    
    Bird* bird = RL_MALLOC(sizeof(Bird));

    bird->state = CHAR_STATE_MID;
    bird->textureUp = birdUpTexture;
    bird->textureMid = birdMidTexture;
    bird->textureDown = birdDownTexture;
    bird->curTex = birdMidTexture;
    bird->angle = 0;
    bird->speed = 25;
    bird->speedRotate = 0;
    bird->rectangle = REC(0, 0, (float)birdUpTexture.width, (float)birdUpTexture.height);
    bird->des = REC(getContext()->widthScreen / 6, getContext()->heightScreen / 2, (float)birdUpTexture.width, (float)birdUpTexture.height);
    bird->contex = getContext();
    bird->isFall = false;

    bird->wing = LoadSound(WING_AUDIO);  
    bird->hit = LoadSound(HIT_AUDIO);  
    bird->die = LoadSound(DIE_AUDIO);  
    bird->point = LoadSound(POINT_AUDIO);  

    return bird;
}

void releaseBird(Bird *c) {
    UnloadSound(c->wing);
    UnloadSound(c->hit);
    UnloadSound(c->die);
    UnloadSound(c->point);
    UnloadTexture(c->textureUp);
    UnloadTexture(c->textureMid);
    UnloadTexture(c->textureDown);
    RL_FREE(c);
}

void drawBird(Bird *bird, float frameTime) {
    
    updateAnimation(bird);
    DrawTexturePro(bird->curTex, bird->rectangle, bird->des, IVEC2, bird->angle, RAYWHITE);
    mElapsedTime += frameTime;
    mElapsedRotate += frameTime;
}

void playFallBird(Bird *bird) {
    if(!bird->isFall) {
        bird->isFall = true;
        bird->speed = 0;
        if(!isPlaySoundHit) {
            isPlaySoundHit = true;
            PlaySound(bird->hit);
        }
    }
}

void playEarnPoint(Bird *bird) {
    PlaySound(bird->point);
}

void jumbBird(Bird *bird) {
    bird->speed = JUMP_FORCE;
    bird->angle = 0.0f;
    bird->speedRotate = -1.5;
    if(!isStart) {
        isStart = true;
    } 
}

int inputControl(Bird *bird, int baseY,float frameTime)
{   
    int key = -1;
    if(bird->contex->state != Stop) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            key = MOUSE_BUTTON_LEFT;
            jumbBird(bird);
            PlaySound(bird->wing);
        } 
    }

    if(!isStart) return key;
    
    if(bird->des.y + bird->des.height >= baseY) {
        if(!isPlaySoundDie) {
            isPlaySoundDie = true;
            PlaySound(bird->die);
        }
        return key;
    }
    // Apply gravity
    bird->speed += GRAVITY;
    float oldY = bird->des.y;
    bird->des.y += bird->speed;

    bool isUp = oldY > bird->des.y;
    if(isUp) {
        bird->speedRotate += acceleration;
    } else if(lastDirection && !isUp) {
        // bird->angle = 0.0f;
        bird->speedRotate = 1.5;
    } else {
        bird->speedRotate -= acceleration;
    }
    bird->angle += bird->speedRotate;
    
    if(bird->isFall) {
        bird->speedRotate += ACCELERATION_ROTATE;
        bird->angle += bird->speedRotate;
        bird->angle = fmin(45, bird->angle);
    }
    lastDirection = isUp;
    return key;
}
