#include "score.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Texture2D createTexture(char* path) {
    Image img = LoadImage(path);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

static Texture2D toTexture(Score *score,char c) {
    switch (c)
    {
    case '0':
        return score->zero;
    case '1':
        return score->one;
    case '2':
        return score->two;
    case '3':
        return score->three;
    case '4':
        return score->four;
    case '5':
        return score->five;
    case '6':
        return score->six;
    case '7':
        return score->seven;
    case '8':
        return score->eight;
    case '9':
        return score->nine;
    default:
        return score->nine;
    }
}

Score* makeScore() {
    Score* score = RL_MALLOC(sizeof(Score));

    score->zero = createTexture(SCORE_ZERO);
    score->one = createTexture(SCORE_ONE);
    score->two = createTexture(SCORE_TWO);
    score->three = createTexture(SCORE_THREE);
    score->four = createTexture(SCORE_FOUR);
    score->five = createTexture(SCORE_FIVE);
    score->six = createTexture(SCORE_SIX);
    score->seven = createTexture(SCORE_SEVEN);
    score->eight = createTexture(SCORE_EIGHT);
    score->nine = createTexture(SCORE_NINE);

    score->contex = getContext();
    score->score = 0;

    return score;
}

void drawScore(Score *score, int num) {
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%d", num);
    int len = strlen(buffer);
    
    if(len == 1) {
        Texture2D tex = toTexture(score, buffer[0]);
        int y = score->contex->heightScreen / 6;
        int x = score->contex->widthScreen / 2 - tex.width / 2;
        DrawTexturePro(tex, REC(0,0,tex.width, tex.height), REC(x,y,tex.width, tex.height), IVEC2, 0, RAYWHITE);
    } else if(len == 2) {
        Texture2D tex1 = toTexture(score, buffer[0]);
        Texture2D tex2 = toTexture(score, buffer[1]);
        int y = score->contex->heightScreen / 6;
        int x = score->contex->widthScreen / 2 - ((tex1.width + tex2.width) / 2);
        DrawTexturePro(tex1, REC(0,0,tex1.width, tex1.height), REC(x,y,tex1.width, tex1.height), IVEC2, 0, RAYWHITE);
        DrawTexturePro(
            tex2, REC(0,0,tex2.width, tex2.height), 
            REC(x + tex1.width, y, tex2.width, tex2.height), IVEC2, 0, RAYWHITE
        );
    } else if(len == 3) {
        Texture2D tex1 = toTexture(score, buffer[0]);
        Texture2D tex2 = toTexture(score, buffer[1]);
        Texture2D tex3 = toTexture(score, buffer[2]);

        int y = score->contex->heightScreen / 6;
        int x = score->contex->widthScreen / 2 - ((tex1.width + tex2.width + tex3.width) / 2);

        DrawTexturePro(tex1, REC(0,0,tex1.width, tex1.height), REC(x,y,tex1.width, tex1.height), IVEC2, 0, RAYWHITE);
        DrawTexturePro(tex2,
            REC(0,0,tex2.width, tex2.height), 
            REC(x + tex1.width,y,tex2.width, tex2.height), IVEC2, 0, RAYWHITE
        );
        DrawTexturePro(
            tex3, REC(0,0,tex3.width, tex3.height), REC(x + tex1.width + tex2.width, y,tex3.width, tex3.height), IVEC2, 0, RAYWHITE
        );
    }
}
void releaseScore(Score *score) {
    UnloadTexture(score->zero);
    UnloadTexture(score->one);
    UnloadTexture(score->two);
    UnloadTexture(score->three);
    UnloadTexture(score->four);
    UnloadTexture(score->five);
    UnloadTexture(score->six);
    UnloadTexture(score->seven);
    UnloadTexture(score->eight);
    UnloadTexture(score->nine);
}