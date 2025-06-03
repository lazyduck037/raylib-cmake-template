#include "pipe.h"
#include "context.h"
#include "define.h"

Texture2D bottomPipeTex;
Texture2D topPipeTex;
int screenHeight = 0;
int screenWidth = 0;
int mBaseHeight = 0;
float mSpeed = 0;
int lastX = 0;

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

void makePipes(const char *pipe, int baseHeight, float speed, Pipe ***outPipes, int *outNumber)
{
    Pipe **pipes;
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

    int numberPipe = (screenHeight / DISTANCE_BETWEEN_PIPE) + 1;
    pipes = RL_MALLOC(numberPipe * sizeof(Pipe *));
    for (int i = 0; i < numberPipe; i++) {
        pipes[i] = RL_MALLOC(sizeof(Pipe));
        initPipe(pipes[i], i * 200);
    }
    *outPipes = pipes;
    *outNumber = numberPipe;
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

void drawPipe(Pipe **pipes,int numberPipe,float frameTime) 
{
    for (int i = 0; i < numberPipe; i++) {
        drawTopAndBotPipe(pipes[i], frameTime);
        if(pipes[i]->top.x < -pipes[i]->top.width) {
            Pipe *last = pipes[numberPipe - 1];
            if(lastX == 0) {
                lastX = last->top.x + 200;
            }
            pipes[i]->top.x = lastX;
            pipes[i]->bottom.x = lastX;
        }
    }   
}

void releasePipe(Pipe **pipes, int numberPipe) {
    for (int i = 0; i < numberPipe; i++) {
        RL_FREE(pipes[i]);
    }
    RL_FREE(pipes);
    UnloadTexture(topPipeTex);
    UnloadTexture(bottomPipeTex);
}
