#include "ui_manager.h"
#include "ui.h"


struct ui_manager *ui_manager_create(struct input_manager *im) {
    struct ui_manager *uim;

    uim = calloc(1, sizeof(*uim));

    uim->l_ui = sf_list_create(sizeof(struct ui *));
    uim->im = im;

    return uim;
}

void ui_manager_update(struct ui_manager *uim) {
    static int mb1_state = KEY_UP;

    if (uim->im->mouse.mb1.state == KEY_DOWN && mb1_state == KEY_UP) {
        int x = uim->im->mouse.mb1.x;
        int y = uim->im->mouse.mb1.y;

        mb1_state = KEY_DOWN;

        SF_LIST_BEGIN(uim->l_ui, struct ui *, ppui);
            struct ui *ui = *ppui;
            if (ui->on_click && sf_rect_iscontain(&ui->area, x, y)) {
                ui->on_click(ui, x - ui->area.x, y - ui->area.y);
            }
        SF_LIST_END();
    } else if (uim->im->mouse.mb1.state == KEY_UP) {
        mb1_state = KEY_UP;
    }
}

void ui_manager_render(struct ui_manager *uim) {
    SF_LIST_BEGIN(uim->l_ui, struct ui *, ppui);
        struct ui *ui = *ppui;
        if (ui->background) {
            sf_texture_draw(ui->background, ui->area.x, ui->area.y);
        }
        /*
         * can draw wireframe here
         */
        if (ui->on_render) {
            glPushMatrix();
            glTranslatef(ui->area.x, ui->area.y, 0.0f);
            ui->on_render(ui);
            glPopMatrix();
        }
    SF_LIST_END();
}
