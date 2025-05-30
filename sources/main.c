#include "raylib.h"
#include "background.h"
#include "resource.h"
#include "context.h"

#define SCREEN_WIDTH (720)
#define SCREEN_HEIGHT (1080)

#define WINDOW_TITLE "Window title"

Rectangle fitScreen(int widthTex, int heightTex, int wScr, int hScr);
float fitScale(int widthTex, int heightTex, int wScr, int hScr);
void InitContex();
int main(void)
{
    InitContex();
    int widthScreen = GetContext()->widthScreen;
    int heightScreen = GetContext()->heightScreen;

    Background *bg = makeBackGround(BACKGROUND_DAY, 300);
    bg->width = widthScreen;
    bg->height = heightScreen;

    Background *baseBg = makeBackGround(BACKGROUND_BASE, 300);
    baseBg->pos.x = 0;
    baseBg->pos.y = heightScreen - baseBg->tex.height;
    baseBg->width = widthScreen;


    SetTargetFPS(30);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            float frameTime = GetFrameTime();
            
            BackgroundDraw(bg, frameTime);
            BackgroundDraw(baseBg, frameTime);
            // DrawFPS(widthScreen - 25, 0);
          
        EndDrawing();
    }
    
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

void InitContex()
{
    GetContext()->widthScreen = SCREEN_WIDTH;
    GetContext()->heightScreen = SCREEN_HEIGHT;
    GetContext()->title = WINDOW_TITLE;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

}