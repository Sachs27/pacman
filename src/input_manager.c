#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "sf_list.h"
#include "input_manager.h"


static struct sf_list *l_input_manager = NULL;

static void handle_mouse_pos(struct input_manager *im,
                             double xpos, double ypos) {
    im->mouse.x = (int) xpos;
    im->mouse.y = (int) ypos;
}

static void on_mouse_pos(GLFWwindow *handle, double xpos, double ypos) {
    SF_LIST_BEGIN(l_input_manager, struct input_manager *, pim);
        if ((*pim)->win->handle == handle) {
            handle_mouse_pos(*pim, xpos, ypos);
        }
    SF_LIST_END();
}

static void handle_mouse_button(struct input_manager *im,
                                int button, int action, int mods) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_1:
        if (action == GLFW_PRESS) {
            im->mouse.mb1.state = KEY_DOWN;
            im->mouse.mb1.x = im->mouse.x;
            im->mouse.mb1.y = im->mouse.y;
        } else if (action == GLFW_RELEASE) {
            im->mouse.mb1.state = KEY_UP;
        }
        break;
    }
}

static void on_mouse_button(GLFWwindow *handle,
                            int button, int action, int mods) {
    SF_LIST_BEGIN(l_input_manager, struct input_manager *, pim);
        if ((*pim)->win->handle == handle) {
            handle_mouse_button(*pim, button, action, mods);
        }
    SF_LIST_END();
}

struct input_manager *input_manager_create(struct sf_window *win) {
    struct input_manager *im;

    if (l_input_manager == NULL) {
        l_input_manager = sf_list_create(sizeof(im));
    }

    im = calloc(1, sizeof(*im));
    im->win = win;

    sf_list_push(l_input_manager, &im);

    glfwSetMouseButtonCallback(win->handle, on_mouse_button);
    glfwSetCursorPosCallback(win->handle, on_mouse_pos);

    return im;
}
