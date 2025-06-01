#include "bird.h"
#include "define.h"
#include "context.h"

#define CHAR_STATE_UP 2
#define CHAR_STATE_MID 1
#define CHAR_STATE_DOWN 0

#define JUMP_FORCE -17.0f
#define GRAVITY 0.8f


float mElapsedTime = 0.0f;
bool isStart = false;
int frameAnim = 0;

static void updateAnimation(Bird *bird) {
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

    bird->location = VEC2(60.0f, getContext()->heightScreen / 2);
    bird->state = CHAR_STATE_MID;
    bird->textureUp = birdUpTexture;
    bird->textureMid = birdMidTexture;
    bird->textureDown = birdDownTexture;
    bird->curTex = birdMidTexture;
    bird->angle = 0;
    bird->speed = 25;
    bird->rectangle = REC(0, 0, (float)birdUpTexture.width, (float)birdUpTexture.height);

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
    Rectangle currentRectangle = REC(bird->location.x, bird->location.y, bird->rectangle.width, bird->rectangle.height);
    DrawTexturePro(bird->curTex, bird->rectangle, currentRectangle, IVEC2, 0, RAYWHITE);
    mElapsedTime += frameTime;
}

void inputControl(Bird *bird, float frameTime)
{   
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        bird->speed = JUMP_FORCE;
        isStart = true;
    } 

    if(!isStart) return;
    
    // Apply gravity
    bird->speed += GRAVITY;
    bird->location.y += bird->speed;
}