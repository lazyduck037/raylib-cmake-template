#include "raylib.h"
#include "background.h"
#include "bird.h"
#include "resource.h"
#include "context.h"
#include "pipe.h"
#include "define.h"
#include "score.h"

Rectangle fitScreen(int widthTex, int heightTex, int wScr, int hScr);
float fitScale(int widthTex, int heightTex, int wScr, int hScr);

void configContex()
{
    getContext()->widthScreen = WIDTH_SCREEN;
    getContext()->heightScreen = HEIGHT_SCREEN;
    getContext()->title = "window";
    getContext()->fps = 60;
}

int main(void)
{
    configContex();
    
    int widthScreen = getContext()->widthScreen;
    int heightScreen = getContext()->heightScreen;
    char *title = getContext()->title;
    int fps = getContext()->fps;

    InitWindow(widthScreen, heightScreen, title);
    InitAudioDevice();

    Background *bg = makeBackGround(BACKGROUND_DAY, 300);
    bg->width = widthScreen;
    bg->height = heightScreen;

    Background *baseBg = makeBackGround(BACKGROUND_BASE, 300);
    baseBg->pos.x = 0;
    baseBg->pos.y = heightScreen - baseBg->tex.height;
    baseBg->width = widthScreen;

    Bird *bird = makeBird(BLUE_BIRD_UPFLAP, BLUE_BIRD_MIDFLAP, BLUE_BIRD_DOWNFLAP);
   
    Pipe **pipes;
    int numberPipe;
    makePipes(PIPE_SCREEN, baseBg->tex.height, 300, &pipes, &numberPipe);
    int baseY = heightScreen - baseBg->height;

    Score* score = makeScore();
    int numberScore = 0;

    SetTargetFPS(fps);

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);
            float frameTime = GetFrameTime();
            
            BackgroundDraw(bg, frameTime);
            BackgroundDraw(baseBg, frameTime);
           
            drawPipe(pipes, numberPipe, frameTime);
            drawScore(score, numberScore);
            drawBird(bird, frameTime);
            inputControl(bird, baseY, frameTime);

            enum Collision result = checkHit(pipes, numberPipe, bird);
            if(result == HitPipe || result == HitGround || result == HitSky) {
                playFallBird(bird);
                getContext()->state = Stop;
            } else if(result == PassPipe) {
                if(numberScore < MAX_SCORE){
                    numberScore++;
                }
                playEarnPoint(bird);
            }

            DrawFPS(widthScreen - 25, 0);
          
        EndDrawing();
    }

    ReleaseBackGround(baseBg);
    ReleaseBackGround(bg);
    releaseBird(bird);
    releasePipe(pipes, numberPipe);
    releaseScore(score);
    CloseAudioDevice();    // Close audio device
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