#ifndef EDITOR_H
#define EDITOR_H


#include "sf_rect.h"
#include "sf_window.h"
#include "ui_window.h"
#include "map.h"


#define EDITOR_WINDOW_W 800
#define EDITOR_WINDOW_H 528


struct mouse_button {
    int ispressed;
    int x;
    int y;
};

struct editor {
    struct sf_window   *window;
    struct map         *map;
    struct ui_window   *map_window;

    struct tileset     *ts;
    struct ui_window   *ts_window;
    int                 ts_col;     /* selected tile */
    int                 ts_row;
    int                 ts_n;

    struct {
        int x;
        int y;
        struct mouse_button lb;
    } mouse;
};


#endif /* EDITOR_H */
