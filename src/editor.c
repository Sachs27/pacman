#include <stdio.h>
#include <unistd.h>

#include <GLFW/glfw3.h>

#include "sf.h"
#include "sf_frame.h"
#include "sf_window.h"
#include "map.h"


static int init_gl(struct sf_window *win) {
    fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, win->w, win->h, 0.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return 0;
}

int main(int argc, char *argv[]) {
    struct sf_window *win;
    struct map *m;


    if (sf_init(argc, argv) != 0) {
        fprintf(stderr, "sf_init error\n");
        return -1;
    }

    win = sf_window_create("Pacman - Map Editor", 640, 480);

    init_gl(win);
    m = map_create("data/maps/classic", 40, 20, 16, 16);
    map_set_tileset(m, 0, "data/textures/walls.png", 16, 16);
    map_set_tile(m, 1, 1, 0, 1);
    map_set_tile(m, 1, 2, 0, 0);
    map_set_tile(m, 1, 3, 0, 2);

    map_save(m);
#if 0
    m = map_load("data/maps/classic");
#endif

    while (sf_window_isopen(win)) {
        glClear(GL_COLOR_BUFFER_BIT);

        map_draw(m);

        glfwSwapBuffers(win->handle);
        glfwPollEvents();
    }

    /*sf_uninit();*/

    return 0;
}
