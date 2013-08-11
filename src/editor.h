#ifndef EDITOR_H
#define EDITOR_H


#include "sf_rect.h"
#include "sf_window.h"
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
    struct sf_rect      map_rect;

    struct tileset     *ts;
    struct sf_rect      ts_rect;
    int                 ts_col;
    int                 ts_row;
    int                 ts_n;

    struct {
        int x;
        int y;
        struct mouse_button lb;
    } mouse;
};

extern struct editor editor;


#endif /* EDITOR_H */
