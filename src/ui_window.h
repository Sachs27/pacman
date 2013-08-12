#ifndef UI_WINDOW_H
#define UI_WINDOW_H


#include "sf_rect.h"
#include "sf_texture.h"


struct ui_window {
    char               *title;
    struct sf_rect      rect;

    void (*on_render)(struct ui_window *);
    void (*on_click)(struct ui_window *, int, int);
};


struct ui_window *ui_window_create(const char *title, struct sf_rect *rect);
void ui_window_render(struct ui_window *w);


#endif /* UI_WINDOW_H */
