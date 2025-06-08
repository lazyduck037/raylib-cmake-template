#include "bird.h"
#include "define.h"
#include "context.h"
#include <math.h>    

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

static void updateAnimation(Bird *bird) {
    if(bird->contex->state != Start) {
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
    bird->des = REC(60.0f, getContext()->heightScreen / 2, (float)birdUpTexture.width, (float)birdUpTexture.height);
    bird->contex = getContext();
    bird->isFall = false;
    return bird;
}

void releaseBird(Bird *c) {
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

void fallBird(Bird *bird) {
    if(!bird->isFall) {
        bird->isFall = true;
        bird->speed = 0;
    }
}

void inputControl(Bird *bird, int baseY,float frameTime)
{   
    if(bird->contex->state != Stop) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            bird->speed = JUMP_FORCE;
            isStart = true;
        } 
    }

    if(!isStart) return;
    
    if(bird->des.y + bird->des.height >= baseY) {
        return;
    }
    // Apply gravity
    bird->speed += GRAVITY;
    bird->des.y += bird->speed;

    if(bird->isFall) {
        bird->speedRotate += ACCELERATION_ROTATE;
        bird->angle += bird->speedRotate;
        bird->angle = fmin(45, bird->angle);
    }
}
