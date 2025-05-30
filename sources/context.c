#include "context.h"

Context g_context = {
    .widthScreen = 0,
    .heightScreen = 0.0f,
    .title = "title"
};


Context* GetContext(void) {
    return &g_context;
}