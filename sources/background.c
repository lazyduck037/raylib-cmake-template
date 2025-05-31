#include "background.h"
#include "define.h"

Background* makeBackGround(const char* source, int speed) 
{
    Image backgroundImage = LoadImage(source);
    Texture2D backgroundTexture = LoadTextureFromImage(backgroundImage);
    UnloadImage(backgroundImage);

    Background* bg = RL_MALLOC(sizeof(Background));

    bg->speed = speed;
    bg->tex = backgroundTexture;
    bg->context = getContext();
    bg->pos = IVEC2;
    bg->width = backgroundTexture.width;
    bg->height = backgroundTexture.height;
    bg->currentPosMove = 0;
    return bg;
}


void BackgroundDraw(Background *bg, float frameTime) {
    float frameSpeed = frameTime * bg->speed;
    int widthScreen = bg->context->widthScreen;

    DrawTexturePro(bg->tex, 
        REC(bg->currentPosMove, 0, bg->tex.width, bg->tex.height), 
        REC(bg->pos.x,bg->pos.y, bg->width, bg->height), IVEC2, 0, RAYWHITE
    );
    bool isMove = bg->speed > 0;
    if(isMove) {
        bg->currentPosMove += (frameSpeed/2);
        if(bg->currentPosMove >= widthScreen)
            bg->currentPosMove = 0;
    }
}

void ReleaseBackGround(Background *bg) 
{
    RL_FREE(bg);
}
