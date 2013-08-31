#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H


#include "sf_window.h"


enum key_state {
    KEY_UP = 0,
    KEY_DOWN = 1,
};

struct mouse_button {
    enum key_state state;
    int x;
    int y;
};

/*
struct keyboard_key {
    enum sf_key_state state;
    int code;
};
*/

struct input_manager {
    struct {
        int x, y;
        struct mouse_button mb1, mb2, mb3;
    } mouse;

    struct sf_window *win;
};


struct input_manager *input_manager_create(struct sf_window *win);


#endif /* INPUT_MANAGER_H */
