#ifndef UI_H
#define UI_H


#include "sf_rect.h"
#include "sf_texture.h"


struct ui;

typedef void (ui_on_render_t)(struct ui *);
typedef void (ui_on_click_t)(struct ui *, int, int);


struct ui {
    struct sf_texture  *background;
    struct sf_rect      area;

    ui_on_render_t  *on_render;
    ui_on_click_t   *on_click;
};


struct ui *ui_create(int x, int y, int w, int h);


inline static void ui_set_background(struct ui *win,
                                            struct sf_texture *texture) {
    win->background = texture;
    win->area.w = texture->w;
    win->area.h = texture->h;
}

inline static void ui_on_render(struct ui *win,
                                       ui_on_render_t *cb) {
    win->on_render = cb;
}

inline static void ui_on_click(struct ui *win,
                                      ui_on_click_t *cb) {
    win->on_click = cb;
}


#endif /* UI_H */
