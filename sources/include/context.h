#ifndef CONTEXT_H
#define CONTEXT_H
enum GameState {
    Menu,
    Guide,
    Start,
    Stop,
    End
};

struct Context {
    int widthScreen;
    int heightScreen;
    char* title;
    int fps;
    enum GameState state;
};
typedef struct Context Context;

Context* getContext(void);

#endif