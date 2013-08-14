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

void ui_render(struct ui *ui) {
    if (ui->background) {
        sf_texture_draw(ui->background, ui->area.x, ui->area.y);
    }
    /* draw wireframe */
    if (ui->on_render) {
        glPushMatrix();
        glTranslatef(ui->area.x, ui->area.y, 0.0f);
        ui->on_render(ui);
        glPopMatrix();
    }
}
