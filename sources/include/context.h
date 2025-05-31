#ifndef CONTEXT_H
#define CONTEXT_H

struct Context {
    int widthScreen;
    int heightScreen;
    char* title;
    int fps;
};
typedef struct Context Context;

Context* getContext(void);

#endif