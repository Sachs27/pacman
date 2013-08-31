#include <stdlib.h>
#include <GL/gl.h>

#include "ui.h"


struct ui *ui_create(int x, int y, int w, int h) {
    struct ui *ui;

    ui = malloc(sizeof(*ui));
    ui->background = NULL;
    ui->area = (struct sf_rect) {x, y, w ,h};
    ui->on_render = NULL;
    ui->on_click = NULL;

    return ui;
}
