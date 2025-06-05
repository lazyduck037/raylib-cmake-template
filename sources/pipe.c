#include "pipe.h"
#include "context.h"
#include "define.h"

Texture2D bottomPipeTex;
int screenHeight = 0;
int screenWidth = 0;
int mBaseHeight = 0;
float mSpeed = 0;
int lastX = 0;

static void initPipe(Pipe *pipe, float x) 
{
    float heightPipe = GetRandomValue(bottomPipeTex.height / 4, bottomPipeTex.height);
    
    pipe->topTex = bottomPipeTex;

    pipe->sourceTop.width = pipe->topTex.width;
    pipe->sourceTop.height = -(screenHeight - heightPipe - PIPE_GAP);
    pipe->sourceTop.x = 0;
    pipe->sourceTop.y = 0;

    pipe->desTop.width = pipe->topTex.width;
    pipe->desTop.height = screenHeight - heightPipe - PIPE_GAP;
    pipe->desTop.x = screenWidth + x;
    pipe->desTop.y = 0;
   

    pipe->botTex = bottomPipeTex;
    pipe->sourceBot.width = pipe->botTex.width;
    pipe->sourceBot.height = heightPipe;
    pipe->sourceBot.x = 0;
    pipe->sourceBot.y = 0;

    
    pipe->desBot.width = pipe->botTex.width;
    pipe->desBot.height = heightPipe;
    pipe->desBot.x = screenWidth + x;
    pipe->desBot.y = screenHeight - pipe->desBot.height - mBaseHeight;
    
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
   
    pipe->desBot.x -= frameSpeed;
    pipe->desTop.x = pipe->desBot.x;
    // pipe->bottom.x -= frameSpeed;
    // pipe->top.x = pipe->bottom.x;

    // DrawTexturePro(pipe->topTex, 
    //     REC(0,0, pipe->topTex.width, pipe->topTex.height), 
    //     pipe->top, op
    //     IVEC2, 0, RAYWHITE
    // );
    // DrawTexturePro(
    //     pipe->botTex, 
    //     REC(0,0, pipe->botTex.width, pipe->botTex.height - 100), 
    //     REC(pipe->bottom.x, pipe->bottom.y + 100, pipe->bottom.width, pipe->bottom.height - 100), 
    //     IVEC2, 0, RAYWHITE
    // );
     DrawTexturePro(
        pipe->botTex, 
        pipe->sourceTop, 
        pipe->desTop, 
        IVEC2, 0, RAYWHITE
    );
    DrawTexturePro(
        pipe->botTex, 
        pipe->sourceBot, 
        pipe->desBot, 
        IVEC2, 0, RAYWHITE
    );

}

void drawPipe(Pipe **pipes,int numberPipe,float frameTime) 
{
    for (int i = 0; i < numberPipe; i++) {
        drawTopAndBotPipe(pipes[i], frameTime);
        if(pipes[i]->desTop.x < -pipes[i]->desTop.width) {
            Pipe *last = pipes[numberPipe - 1];
            if(lastX == 0) {
                lastX = last->desTop.x + 200;
            }
            pipes[i]->desTop.x = lastX;
            pipes[i]->desBot.x = lastX;
        }
    }   
}

void releasePipe(Pipe **pipes, int numberPipe) {
    for (int i = 0; i < numberPipe; i++) {
        RL_FREE(pipes[i]);
    }
    RL_FREE(pipes);
    UnloadTexture(bottomPipeTex);
}
