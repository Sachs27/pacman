#include <stdlib.h>
#include <GL/gl.h>

#include "ui_window.h"


static int UI_WINDOW_TITLE_H = 16;


struct ui_window *ui_window_create(const char *title, struct sf_rect *rect) {
    struct ui_window *w;

    w = malloc(sizeof(*w));
    w->title = strdup(title);
    w->rect = *rect;
    w->on_render = NULL;

    return w;
}

void ui_window_render(struct ui_window *w) {
    /* draw wireframe */
    if (w->on_render) {
        glPushMatrix();
        glTranslatef(w->rect.x, w->rect.y, 0.0f);
        w->on_render(w);
        glPopMatrix();
    }
}
