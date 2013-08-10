#ifndef SF_WINDOW_H
#define SF_WINDOW_H


#include <GLFW/glfw3.h>


typedef GLFWwindow sf_window_handle_t;

struct sf_window {
    char   *title;
    int     w;
    int     h;

    sf_window_handle_t *handle;
};

struct sf_window *sf_window_create(const char *title, int w, int h);
void sf_window_destroy(struct sf_window *win);
int sf_window_isopen(struct sf_window *win);


#endif
