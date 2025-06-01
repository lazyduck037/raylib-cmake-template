#include "pipe.h"
#include "context.h"
#include "define.h"

#define PIPE_GAP 150
#define PIPE_WIDTH 80

const int PIPE_COUNT = 4;
Pipe pipes[5];
Texture2D bottomPipeTex;
Texture2D topPipeTex;
int screenHeight = 0;
int screenWidth = 0;
int mBaseHeight = 0;
float mSpeed = 0;


static void initPipe(Pipe *pipe, float x) 
{
    float gapY = GetRandomValue(PIPE_GAP, screenHeight - PIPE_GAP);
    
    pipe->top.width = topPipeTex.width;
    pipe->top.height = topPipeTex.height;
    pipe->top.x = screenWidth + x;
    pipe->top.y = 0;
    pipe->topTex = topPipeTex;
    
    pipe->bottom.width = bottomPipeTex.width;
    pipe->bottom.height = bottomPipeTex.height;
    pipe->bottom.x = screenWidth + x;
    pipe->bottom.y = screenHeight - pipe->bottom.height - mBaseHeight;;
    pipe->botTex = bottomPipeTex;
    
    pipe->passed = false; 
}

void makePipes(const char *pipe, int baseHeight, float speed) 
{
    screenWidth = getContext()->widthScreen;
    screenHeight = getContext()->heightScreen;
    mBaseHeight = baseHeight;
    mSpeed = speed;

    Image pipeBotImage = LoadImage(pipe);
    bottomPipeTex = LoadTextureFromImage(pipeBotImage);
    UnloadImage(pipeBotImage);

    Image pipeTopImage = LoadImage(pipe);
    ImageRotate(&pipeTopImage, 180);
    topPipeTex = LoadTextureFromImage(pipeTopImage);
    UnloadImage(pipeTopImage);

    for (int i = 0; i < PIPE_COUNT; i++) {
        initPipe(&pipes[i], i * 200);
    }
    // initPipe(&pipes[0], 0);
    // initPipe(&pipes[1], 200);
}


static void drawTopAndBotPipe(Pipe *pipe, float frameTime) {
    
    float frameSpeed = (frameTime * mSpeed) / 2;
   
    pipe->bottom.x -= frameSpeed;
    pipe->top.x = pipe->bottom.x;

    DrawTexturePro(pipe->topTex, 
        REC(0,0, pipe->topTex.width, pipe->topTex.height), 
        pipe->top, 
        IVEC2, 0, RAYWHITE
    );
    DrawTexturePro(
        pipe->botTex, 
        REC(0,0, pipe->botTex.width, pipe->botTex.height), 
        pipe->bottom, 
        IVEC2, 0, RAYWHITE
    );

}

void drawPipe(float frameTime) 
{ 
    for (int i = 0; i < PIPE_COUNT; i++) {
        drawTopAndBotPipe(&pipes[i], frameTime);
    }
   
}