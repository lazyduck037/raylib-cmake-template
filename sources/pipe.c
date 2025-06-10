#include "pipe.h"
#include "context.h"
#include "define.h"
#include <stdio.h>
#include <math.h>

Texture2D pipeTex;
int screenHeight = 0;
int screenWidth = 0;
int mBaseHeight = 0;
float mSpeed = 0;
int lastX = 0;

static void initPipe(Pipe *pipe, float x, int rangeOfHeight, int gapHeight, int posGap) 
{
    pipe->sourceTop.width = pipeTex.width;
    pipe->sourceTop.height = -posGap;
    pipe->sourceTop.x = 0;
    pipe->sourceTop.y = 0;

    pipe->desTop.width = pipeTex.width;
    pipe->desTop.height = posGap;
    pipe->desTop.x = screenWidth + x;
    pipe->desTop.y = 0;

    pipe->sourceBot.width = pipeTex.width;
    pipe->sourceBot.height = rangeOfHeight - posGap - gapHeight;
    pipe->sourceBot.x = 0;
    pipe->sourceBot.y = 0;

    pipe->desBot.width = pipeTex.width;
    pipe->desBot.height = rangeOfHeight - posGap - gapHeight;
    pipe->desBot.x = screenWidth + x;
    pipe->desBot.y = rangeOfHeight - pipe->desBot.height;

    pipe->isPass = false;
    pipe->contex = getContext();
}

static void updatePipe(Pipe *pipe, float x, int pipeGap, int heightPipe) 
{
    pipe->desTop.x = x;
    pipe->desBot.x = x;

    int rangeOfHeight = screenHeight - mBaseHeight;
    int gapHeight = GetRandomValue(MIN_PIPE_GAP, MAX_PIPE_GAP);
    int max = rangeOfHeight - MIN_HEIGHT_PIPE - gapHeight;

    int posGap = GetRandomValue(MIN_HEIGHT_PIPE, max);
    
    pipe->sourceTop.height = -posGap;
    pipe->desTop.height = posGap;
    
    pipe->desBot.height = rangeOfHeight - posGap - gapHeight;
    pipe->sourceBot.height = rangeOfHeight - posGap - gapHeight;
    pipe->desBot.y = rangeOfHeight - pipe->desBot.height;
    pipe->isPass = false;
    // pipe->sourceTop.height = -(screenHeight - heightPipe - pipeGap);
    // pipe->desTop.height = screenHeight - heightPipe - pipeGap;

    // pipe->desBot.height = heightPipe;
    // pipe->sourceBot.height = heightPipe;

    // pipe->desBot.y = screenHeight - pipe->desBot.height - mBaseHeight;
}


void makePipes(const char *pipe, int baseHeight, float speed, Pipe ***outPipes, int *outNumber)
{
    Pipe **pipes;
    screenWidth = getContext()->widthScreen;
    screenHeight = getContext()->heightScreen;
    mBaseHeight = baseHeight;
    mSpeed = speed;

    Image pipeBotImage = LoadImage(pipe);
    pipeTex = LoadTextureFromImage(pipeBotImage);
    UnloadImage(pipeBotImage);

    int numberPipe = (screenWidth / DISTANCE_BETWEEN_PIPE) + 1;
    pipes = RL_MALLOC(numberPipe * sizeof(Pipe *));
    int rangeOfHeight = screenHeight - baseHeight;
    int min = 15;
   
    for (int i = 0; i < numberPipe; i++) {
        pipes[i] = RL_MALLOC(sizeof(Pipe));
       
        int gapHeight = GetRandomValue(MIN_PIPE_GAP, MAX_PIPE_GAP);
        int max = rangeOfHeight -  15 - gapHeight;
  
        int posGap = GetRandomValue(min, max);
        initPipe(pipes[i], i * DISTANCE_BETWEEN_PIPE, rangeOfHeight, gapHeight, posGap);
    }
    *outPipes = pipes;
    *outNumber = numberPipe;
}


static void drawTopAndBotPipe(Pipe *pipe, float frameTime) {
    
    if(pipe->contex->state == Start) {
        float frameSpeed = (frameTime * mSpeed) / 2;
    
        pipe->desBot.x -= frameSpeed;
        pipe->desTop.x = pipe->desBot.x;
    }
    DrawTexturePro(
        pipeTex, 
        pipe->sourceTop, 
        pipe->desTop, 
        IVEC2, 0, RAYWHITE
    );
    DrawTexturePro(
        pipeTex, 
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
               lastX = last->desTop.x + DISTANCE_BETWEEN_PIPE;
            }

            float heightPipe = GetRandomValue(pipeTex.height / 4, pipeTex.height);
            int PIPE_GAP = GetRandomValue(MIN_PIPE_GAP, MAX_PIPE_GAP);
            updatePipe(pipes[i], lastX, PIPE_GAP, heightPipe);
            
        }
    }   
}


enum Collision checkHit(Pipe **pipes, int numPipes,Bird * bird) {
    if((bird->des.y + bird->des.height) > screenHeight - mBaseHeight) {
        return HitGround;
    } else if (bird->des.y <= 0) {
        return HitSky;
    }
    
    for (int i = 0; i < numPipes; i++)
    {
        Pipe *pipe = pipes[i];
        if (CheckCollisionRecs(bird->des, pipes[i]->desTop) ||
            CheckCollisionRecs(bird->des, pipes[i]->desBot)) {
            return HitPipe;
        } else {
            float centerOfPipe = pipe->desTop.x + (pipe->desTop.width / 2);
            float centerOfBird = bird->des.x + bird->des.width / 2;
            if(!pipe->isPass && centerOfPipe <= centerOfBird) {
            pipe->isPass = true;
            return PassPipe;
            }
        }
    }
    return None;
}

void releasePipe(Pipe **pipes, int numberPipe) {
    for (int i = 0; i < numberPipe; i++) {
        RL_FREE(pipes[i]);
    }
    RL_FREE(pipes);
    UnloadTexture(pipeTex);
}
