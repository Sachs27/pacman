#include <stdlib.h>
#include <string.h>

#include "sf_window.h"


struct sf_window *sf_window_create(const char *title, int w, int h) {
    struct sf_window *win;

    win = malloc(sizeof(*win));
    win->title = strdup(title);
    win->w = w;
    win->h = h;

    win->handle = glfwCreateWindow(w, h, title, NULL, NULL);

    if (win->handle == NULL) {
        free(win);
        return NULL;
    }

    glfwMakeContextCurrent(win->handle);

    return win;
}

void sf_window_destroy(struct sf_window *win) {
    glfwDestroyWindow(win->handle);
    free(win->title);
    free(win);
}

int sf_window_isopen(struct sf_window *win) {
    return !glfwWindowShouldClose(win->handle);
}
