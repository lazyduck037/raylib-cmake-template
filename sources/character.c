#include "character.h"
#include "define.h"
#include "context.h"

#define CHAR_STATE_UP 1
#define CHAR_STATE_MID 0
#define CHAR_STATE_DOWN -1

float mElapsedUpTime = 0.0f;
float mElapsedTime = 0.0f;
float mGraviationSpeed = 45.0f;
float Gravity = 2000.0f; //100 pixel / s2

bool isStart = false;

static Vector2 moveUp(Character * character, float frameTime) 
{
    if(character->state != CHAR_STATE_UP)
    {
        character->currentSpeed = 0.0f;
        mElapsedTime = frameTime;
        mElapsedUpTime = frameTime;
    }
    else {
        mElapsedTime = frameTime;
        mElapsedUpTime = mElapsedUpTime + frameTime;
    }

    character->state = CHAR_STATE_UP;
    //v0t + 0.5at2 
    float s = character->currentSpeed * mElapsedTime + 0.5f * Gravity * (mElapsedTime * mElapsedTime);
    character->currentSpeed = character->currentSpeed + Gravity * mElapsedTime;
    DrawText(TextFormat("%f", character->currentSpeed), 300, 80, 30, WHITE);
    DrawText(TextFormat("%f",s), 300, 160, 30, WHITE);
    character->location.y -= s ;
    
    return character->location;
}

void static gravitationModifier(Character * character, float frameTime) {
     if(character->state == CHAR_STATE_UP)
     {
        character->currentSpeed = 0.0f;
        mElapsedTime = frameTime;
     }
    else 
    {
        mElapsedTime = frameTime;
    }
       
    character->state = CHAR_STATE_DOWN;
    //v0t + 0.5at2 
    float s = character->currentSpeed * mElapsedTime + 0.5 * Gravity * (mElapsedTime * mElapsedTime);
    character->currentSpeed = character->currentSpeed + Gravity * mElapsedTime;

    DrawText(TextFormat("%f",character->currentSpeed), 300, 80, 30, WHITE);
    DrawText(TextFormat("%f",s), 300, 160, 30, WHITE);
    character->location.y += s ;
}


Character* makeCharacter(const char *upTex,const char *midTex, const char *dowTex) 
{
    Image characterImage = LoadImage(upTex);
    ImageResize(&characterImage, (int)(characterImage.width*1.5),(int)(characterImage.height*1.5));
    Texture2D characterUpTexture = LoadTextureFromImage(characterImage);
    UnloadImage(characterImage);

    characterImage = LoadImage(midTex);
    ImageResize(&characterImage, (int)(characterImage.width*1.5),(int)(characterImage.height*1.5));
    Texture2D characterMidTexture = LoadTextureFromImage(characterImage);
    UnloadImage(characterImage);

    characterImage = LoadImage(dowTex);
    ImageResize(&characterImage, (int)(characterImage.width*1.5),(int)(characterImage.height*1.5));
    Texture2D characterDownTexture = LoadTextureFromImage(characterImage);
    UnloadImage(characterImage);
    
    Character* character = RL_MALLOC(sizeof(Character));

    character->location = VEC2(60.0f, getContext()->heightScreen / 2);
    character->location = VEC2(60.0f, getContext()->heightScreen / 2);
    character->state = CHAR_STATE_MID;
    character->textureUp = characterUpTexture;
    character->textureMid = characterMidTexture;
    character->textureDown = characterDownTexture;
    character->angle = 0;
    character->speed = 25;
    character->currentSpeed = 0;
    character->currentTime = 0.0f;
    character->rectangle = REC(0, 0, (float)characterUpTexture.width, (float)characterUpTexture.height);

    return character;
}

void releaseCharacter(Character *c) {
    RL_FREE(c);
}

void drawCharacter(Character *character) {
    Texture2D currentTexture;
    switch (character->state) {
        case CHAR_STATE_UP:
            currentTexture = character->textureUp;
            break;
        case CHAR_STATE_DOWN:
            currentTexture = character->textureDown;
            break;
        default:
            currentTexture = character->textureMid;
            break;
    }
    Rectangle currentRectangle = REC(character->location.x, character->location.y, character->rectangle.width, character->rectangle.height);
    DrawTexturePro(currentTexture, character->rectangle, currentRectangle, IVEC2, 0, RAYWHITE);
}

void inputControl(Character *character, float frameTime) 
{   
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        moveUp(character, frameTime);
        isStart = true;
        //  gravitationModifier(character, frameTime);
    } else {
        if(!isStart) return;
        if(character->state == CHAR_STATE_UP && mElapsedUpTime < 0.5) 
        {
            moveUp(character, frameTime);
            // gravitationModifier(character, frameTime);
        } else {
            gravitationModifier(character, frameTime);
           
        }
    }
}