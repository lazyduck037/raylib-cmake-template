#include "raylib.h"

#define SCREEN_WIDTH (720)
#define SCREEN_HEIGHT (1080)

#define WINDOW_TITLE "Window title"

Rectangle fitScreen(int widthTex, int heightTex, int wScr, int hScr);
float fitScale(int widthTex, int heightTex, int wScr, int hScr);
int main(void)
{
    float currentBackgroundPosition = 0;
    int screenWidth = SCREEN_WIDTH;
    int screenHeight = SCREEN_HEIGHT;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    Image backgroundImage = LoadImage(ASSETS_PATH"/background/background-day.png");
    Texture2D backgroundTexture = LoadTextureFromImage(backgroundImage);
    UnloadImage(backgroundImage);

    SetTargetFPS(60);

    float bgScale = fitScale(backgroundTexture.width, backgroundTexture.height, screenWidth, screenHeight);
    Rectangle bgRec = fitScreen(backgroundTexture.width, backgroundTexture.height, screenWidth, screenHeight);
    Rectangle bgSrc = (Rectangle) {0,0,backgroundTexture.width, backgroundTexture.height};
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawTextureRec(backgroundTexture, (Rectangle){0,0,(float)screenWidth, (float)screenHeight}, (Vector2){0,0}, RAYWHITE);
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