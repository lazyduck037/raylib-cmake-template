#include "context.h"

Context g_context = {
    .widthScreen = 0,
    .heightScreen = 0.0f,
    .title = "title",
    .fps = 60,
    .state = Menu
};


Context* getContext(void) {
    return &g_context;
}