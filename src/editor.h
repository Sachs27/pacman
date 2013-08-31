#ifndef EDITOR_H
#define EDITOR_H


#include "sf_rect.h"
#include "sf_window.h"
#include "ui.h"
#include "ui_manager.h"
#include "map.h"


#define EDITOR_WINDOW_W 800
#define EDITOR_WINDOW_H 537


struct editor {
    struct sf_window   *window;

    struct map         *map;
    struct ui          *map_window;

    struct tileset     *ts;
    struct ui          *ts_window;
    int                 ts_col;     /* selected tile */
    int                 ts_row;
    int                 ts_n;

    struct ui          *bt_create;
    struct ui          *bt_destroy;
    struct ui          *bt_prev;
    struct ui          *bt_next;
    struct ui          *bt_save;

    struct input_manager   *im;
    struct ui_manager      *uim;
};


#endif /* EDITOR_H */
