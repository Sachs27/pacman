#ifndef UI_MANAGER_H
#define UI_MANAGER_H


#include "sf_list.h"
#include "input_manager.h"


struct ui_manager {
    struct sf_list *l_ui;
    struct input_manager *im;
};


struct ui_manager *ui_manager_create(struct input_manager *im);
void ui_manager_update(struct ui_manager *uim);
void ui_manager_render(struct ui_manager *uim);

#define ui_manager_add(uim, ppui) sf_list_push_front((uim)->l_ui, (ppui))


#endif /* UI_MANAGER_H */
