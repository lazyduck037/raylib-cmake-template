#include "pipe.h"
#include "context.h"
#include "define.h"

#define PIPE_GAP 150
#define PIPE_WIDTH 80

const int PIPE_COUNT = 4;
Pipe pipes[4];
Texture2D pipeTex;

static void initPipe(Pipe *pipe, float x) 
{
    int heightScreen = getContext()->heightScreen;
    float gapY = GetRandomValue(PIPE_GAP, heightScreen - PIPE_GAP);
    
    pipe->top.x = x;
    pipe->top.y = 0;
    pipe->top.width = PIPE_WIDTH;
    pipe->top.height = gapY - PIPE_GAP/2;
    
    pipe->bottom.x = x;
    pipe->bottom.y = gapY + PIPE_GAP/2;
    pipe->bottom.width = PIPE_WIDTH;
    pipe->bottom.height = heightScreen - (gapY + PIPE_GAP/2);
    
    pipe->passed = false; 
}

void makePipes(const char *pipe) 
{
    int screenWidth = getContext()->widthScreen;
    
    Image pipeImage = LoadImage(pipe);
    pipeTex = LoadTextureFromImage(pipeImage);
    UnloadImage(pipeImage);

    for (int i = 0; i < PIPE_COUNT; i++) {
        initPipe(&pipes[i], screenWidth + i * 200);
    }
}

void drawPipe() 
{
    DrawTexturePro(pipeTex, REC(0,0, pipeTex.width, pipeTex.height), pipes[0].bottom, IVEC2, 0, RAYWHITE);
}