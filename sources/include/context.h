#ifndef CONTEXT_H
#define CONTEXT_H

struct Context {
    int widthScreen;
    int heightScreen;
    char* title;
};
typedef struct Context Context;

Context* GetContext(void);

#endif