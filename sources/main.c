#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BIRD_SIZE 30
#define PIPE_WIDTH 80
#define PIPE_GAP 150
#define GRAVITY 0.5f
#define JUMP_FORCE -8.0f
#define PIPE_SPEED 3.0f

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Rectangle rect;
    bool isAlive;
} Bird;

typedef struct {
    Rectangle top;
    Rectangle bottom;
    bool passed;
} Pipe;

typedef enum {
    MENU,
    PLAYING,
    GAME_OVER
} GameState;

// Function declarations
void InitBird(Bird *bird);
void UpdateBird(Bird *bird);
void DrawBird(Bird *bird);
void InitPipe(Pipe *pipe, float x);
void UpdatePipes(Pipe pipes[], int pipeCount, int *score);
void DrawPipes(Pipe pipes[], int pipeCount);
bool CheckCollision(Bird *bird, Pipe pipes[], int pipeCount);
void ResetGame(Bird *bird, Pipe pipes[], int pipeCount, int *score);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird");
    SetTargetFPS(60);
    
    Bird bird;
    const int PIPE_COUNT = 4;
    Pipe pipes[PIPE_COUNT];
    int score = 0;
    int highScore = 0;
    GameState gameState = MENU;
    
    // Initialize game objects
    InitBird(&bird);
    for (int i = 0; i < PIPE_COUNT; i++) {
        InitPipe(&pipes[i], SCREEN_WIDTH + i * 200);
    }
    
    while (!WindowShouldClose()) {
        // Update
        switch (gameState) {
            case MENU:
                if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    gameState = PLAYING;
                    ResetGame(&bird, pipes, PIPE_COUNT, &score);
                }
                break;
                
            case PLAYING:
                UpdateBird(&bird);
                UpdatePipes(pipes, PIPE_COUNT, &score);
                
                if (CheckCollision(&bird, pipes, PIPE_COUNT) || 
                    bird.position.y > SCREEN_HEIGHT - BIRD_SIZE/2 ||
                    bird.position.y < BIRD_SIZE/2) {
                    gameState = GAME_OVER;
                    bird.isAlive = false;
                    if (score > highScore) {
                        highScore = score;
                    }
                }
                break;
                
            case GAME_OVER:
                if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    gameState = PLAYING;
                    ResetGame(&bird, pipes, PIPE_COUNT, &score);
                }
                if (IsKeyPressed(KEY_M)) {
                    gameState = MENU;
                }
                break;
        }
        
        // Draw
        BeginDrawing();
        ClearBackground(SKYBLUE);
        
        // Draw background gradient
        DrawRectangleGradientV(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SKYBLUE, BLUE);
        
        switch (gameState) {
            case MENU:
                DrawText("FLAPPY BIRD", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 - 100, 40, WHITE);
                DrawText("Press SPACE or CLICK to start", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 30, 20, WHITE);
                DrawText(TextFormat("High Score: %d", highScore), SCREEN_WIDTH/2 - 70, SCREEN_HEIGHT/2 + 20, 20, WHITE);
                break;
                
            case PLAYING:
                DrawPipes(pipes, PIPE_COUNT);
                DrawBird(&bird);
                DrawText(TextFormat("Score: %d", score), 10, 10, 30, WHITE);
                break;
                
            case GAME_OVER:
                DrawPipes(pipes, PIPE_COUNT);
                DrawBird(&bird);
                DrawText("GAME OVER", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 50, 40, RED);
                DrawText(TextFormat("Final Score: %d", score), SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2, 20, WHITE);
                DrawText(TextFormat("High Score: %d", highScore), SCREEN_WIDTH/2 - 70, SCREEN_HEIGHT/2 + 30, 20, WHITE);
                DrawText("Press SPACE to restart", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 60, 20, WHITE);
                DrawText("Press M for menu", SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2 + 90, 20, WHITE);
                break;
        }
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

void InitBird(Bird *bird) {
    bird->position.x = 100;
    bird->position.y = SCREEN_HEIGHT / 2;
    bird->velocity.x = 0;
    bird->velocity.y = 0;
    bird->rect.width = BIRD_SIZE;
    bird->rect.height = BIRD_SIZE;
    bird->isAlive = true;
}

void UpdateBird(Bird *bird) {
    if (!bird->isAlive) return;
    
    // Handle input
    if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        bird->velocity.y = JUMP_FORCE;
    }
    
    // Apply gravity
    bird->velocity.y += GRAVITY;
    
    // Update position
    bird->position.y += bird->velocity.y;
    
    // Update rectangle for collision
    bird->rect.x = bird->position.x - BIRD_SIZE/2;
    bird->rect.y = bird->position.y - BIRD_SIZE/2;
}

void DrawBird(Bird *bird) {
    Color birdColor = bird->isAlive ? YELLOW : RED;
    
    // Draw bird body
    DrawCircle(bird->position.x, bird->position.y, BIRD_SIZE/2, birdColor);
    
    // Draw bird eye
    DrawCircle(bird->position.x + 8, bird->position.y - 5, 4, WHITE);
    DrawCircle(bird->position.x + 10, bird->position.y - 5, 2, BLACK);
    
    // Draw beak
    Vector2 beakPoints[3] = {
        {bird->position.x + BIRD_SIZE/2, bird->position.y},
        {bird->position.x + BIRD_SIZE/2 + 10, bird->position.y - 3},
        {bird->position.x + BIRD_SIZE/2 + 10, bird->position.y + 3}
    };
    DrawTriangle(beakPoints[0], beakPoints[1], beakPoints[2], ORANGE);
}

void InitPipe(Pipe *pipe, float x) {
    float gapY = GetRandomValue(PIPE_GAP, SCREEN_HEIGHT - PIPE_GAP);
    
    pipe->top.x = x;
    pipe->top.y = 0;
    pipe->top.width = PIPE_WIDTH;
    pipe->top.height = gapY - PIPE_GAP/2;
    
    pipe->bottom.x = x;
    pipe->bottom.y = gapY + PIPE_GAP/2;
    pipe->bottom.width = PIPE_WIDTH;
    pipe->bottom.height = SCREEN_HEIGHT - (gapY + PIPE_GAP/2);
    
    pipe->passed = false;
}

void UpdatePipes(Pipe pipes[], int pipeCount, int *score) {
    for (int i = 0; i < pipeCount; i++) {
        // Move pipes
        pipes[i].top.x -= PIPE_SPEED;
        pipes[i].bottom.x -= PIPE_SPEED;
        
        // Check if pipe passed bird
        if (!pipes[i].passed && pipes[i].top.x + PIPE_WIDTH < 100) {
            pipes[i].passed = true;
            (*score)++;
        }
        
        // Reset pipe when it goes off screen
        if (pipes[i].top.x < -PIPE_WIDTH) {
            InitPipe(&pipes[i], SCREEN_WIDTH);
        }
    }
}

void DrawPipes(Pipe pipes[], int pipeCount) {
    for (int i = 0; i < pipeCount; i++) {
        // Draw pipes with gradient
        DrawRectangleGradientH(pipes[i].top.x, pipes[i].top.y, 
                              pipes[i].top.width, pipes[i].top.height, 
                              GREEN, DARKGREEN);
        DrawRectangleGradientH(pipes[i].bottom.x, pipes[i].bottom.y, 
                              pipes[i].bottom.width, pipes[i].bottom.height, 
                              GREEN, DARKGREEN);
        
        // Draw pipe borders
        DrawRectangleLines(pipes[i].top.x, pipes[i].top.y, 
                          pipes[i].top.width, pipes[i].top.height, BLACK);
        DrawRectangleLines(pipes[i].bottom.x, pipes[i].bottom.y, 
                          pipes[i].bottom.width, pipes[i].bottom.height, BLACK);
        
        // Draw pipe caps
        DrawRectangle(pipes[i].top.x - 5, pipes[i].top.height - 20, 
                     PIPE_WIDTH + 10, 20, DARKGREEN);
        DrawRectangle(pipes[i].bottom.x - 5, pipes[i].bottom.y, 
                     PIPE_WIDTH + 10, 20, DARKGREEN);
    }
}

bool CheckCollision(Bird *bird, Pipe pipes[], int pipeCount) {
    for (int i = 0; i < pipeCount; i++) {
        if (CheckCollisionRecs(bird->rect, pipes[i].top) ||
            CheckCollisionRecs(bird->rect, pipes[i].bottom)) {
            return true;
        }
    }
    return false;
}

void ResetGame(Bird *bird, Pipe pipes[], int pipeCount, int *score) {
    InitBird(bird);
    *score = 0;
    
    for (int i = 0; i < pipeCount; i++) {
        InitPipe(&pipes[i], SCREEN_WIDTH + i * 200);
    }
}