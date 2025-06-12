#ifndef DEFINE_H
#define DEFINE_H

#define IVEC2 (Vector2){0, 0}
#define VEC2(x,y) (Vector2){x, y}
#define REC(x,y,w,h) (Rectangle){x,y, w, h}
#define SimpleDraw(tex, x, y) DrawTexturePro(tex, REC(0, 0, tex.width, tex.height), REC(x, y, tex.width, tex.height), IVEC2, 0, RAYWHITE);

#define MIN_HEIGHT_PIPE 35
#define MAX_PIPE_GAP 240
#define MIN_PIPE_GAP 210
#define DISTANCE_BETWEEN_PIPE 270


// #define JUMP_FORCE -17.0f
#define JUMP_FORCE -15.0f
#define GRAVITY 0.8f
#define WIDTH_SCREEN 720
#define HEIGHT_SCREEN 1080
#define MAX_SCORE 999

#endif