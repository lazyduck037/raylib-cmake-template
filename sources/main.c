#include "raylib.h"
#include "background.h"
#include "character.h"
#include "resource.h"
#include "context.h"

Rectangle fitScreen(int widthTex, int heightTex, int wScr, int hScr);
float fitScale(int widthTex, int heightTex, int wScr, int hScr);

void InitContex()
{
    getContext()->widthScreen = 720;
    getContext()->heightScreen = 1080;
    getContext()->title = "window";
    getContext()->fps = 30;
}

int main(void)
{
    InitContex();
    
    int widthScreen = getContext()->widthScreen;
    int heightScreen = getContext()->heightScreen;
    char *title = getContext()->title;
    int fps = getContext()->fps;

    InitWindow(widthScreen, heightScreen, title);

    Background *bg = makeBackGround(BACKGROUND_DAY, 300);
    bg->width = widthScreen;
    bg->height = heightScreen;

    Background *baseBg = makeBackGround(BACKGROUND_BASE, 300);
    baseBg->pos.x = 0;
     baseBg->pos.y = heightScreen - baseBg->tex.height;
    baseBg->width = widthScreen;

    Character *character = makeCharacter(BLUE_BIRD_UPFLAP, BLUE_BIRD_MIDFLAP, BLUE_BIRD_DOWNFLAP);

    SetTargetFPS(fps);

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);
            float frameTime = GetFrameTime();
            
            BackgroundDraw(bg, frameTime);
            BackgroundDraw(baseBg, frameTime);
            drawCharacter(character);
            inputControl(character, frameTime);

            DrawFPS(widthScreen - 25, 0);
          
        EndDrawing();
    }

    ReleaseBackGround(baseBg);
    ReleaseBackGround(bg);
    releaseCharacter(character);
    CloseWindow();
   
    return 0;
}

Rectangle fitScreen(int widthTex, int heightTex, int wScr, int hScr)
{
    float ratioOfScreen = wScr * 1.0f / hScr;
    float ratioOfBg = widthTex * 1.0f / heightTex;
    // Full width

    int heightTexWillBeIfFullWidth = (int)(wScr * 1.0f / ratioOfBg);
    if (heightTexWillBeIfFullWidth >= hScr)
    {

        // return (Rectangle){0, (hScr - heightTexWillBeIfFullWidth) / 2, wScr, heightTexWillBeIfFullWidth};
          return (Rectangle){0, 0, wScr, heightTexWillBeIfFullWidth};
    }
    else
    {
        int widthTexWillBeIfFullHeight = (int)(hScr * ratioOfBg);
        return (Rectangle){(wScr - widthTexWillBeIfFullHeight) / 2, 0, widthTexWillBeIfFullHeight, hScr};
    }
}

float fitScale(int widthTex, int heightTex, int wScr, int hScr)
{
    float ratioOfScreen = wScr * 1.0f / hScr;
    float ratioOfBg = widthTex * 1.0f / heightTex;
    // Full width

    int heightTexWillBeIfFullWidth = (int)(wScr * 1.0f / ratioOfBg);
    if (heightTexWillBeIfFullWidth >= hScr)
    {
        return wScr * 1.0f / widthTex;
    }
    else
    {
        return hScr * 1.0f / heightTex;
    }
}