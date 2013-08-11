#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include <GLFW/glfw3.h>

#include "sf.h"
#include "sf_frame.h"
#include "sf_window.h"
#include "map.h"
#include "pacman.h"
#include "editor.h"


struct editor editor;


static void on_mouse_pos(GLFWwindow *window, double xpos, double ypos) {
    editor.mouse.x = (int) xpos;
    editor.mouse.y = (int) ypos;
}

static void on_mouse_button(GLFWwindow *window,
                            int button, int action, int mods) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_1:
        if (action == GLFW_PRESS) {
            editor.mouse.lb.ispressed = 1;
            editor.mouse.lb.x= editor.mouse.x;
            editor.mouse.lb.y= editor.mouse.y;
        } else if (action == GLFW_RELEASE) {
            editor.mouse.lb.ispressed = 0;
        }
        break;
    }
}

static void on_window_size(GLFWwindow *window, int width, int height) {
    editor.window->w = width;
    editor.window->h = height;

    glViewport(0, 0, editor.window->w, editor.window->h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, editor.window->w, editor.window->h, 0.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static int init_gl(void) {
    fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, editor.window->w, editor.window->h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, editor.window->w, editor.window->h, 0.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return 0;
}

static void editor_init(void) {
    editor.window = sf_window_create("Pacman - Map Editor",
                                     EDITOR_WINDOW_W, EDITOR_WINDOW_H);
    glfwSetMouseButtonCallback(editor.window->handle, on_mouse_button);
    glfwSetCursorPosCallback(editor.window->handle, on_mouse_pos);
    glfwSetWindowSizeCallback(editor.window->handle, on_window_size);

    init_gl();

    editor.map = map_create("data/maps/classic",
                            PACMAN_MAP_COL, PACMAN_MAP_ROW,
                            PACMAN_MAP_TW, PACMAN_MAP_TH);
    map_set_tileset(editor.map, 0, "data/textures/walls.png", 16, 16);
    editor.map_rect = (struct sf_rect) {
        16, 16,
        editor.map->col * editor.map->tw, editor.map->row * editor.map->th
    };

    editor.ts = editor.map->tss;
    editor.ts_rect = (struct sf_rect) {
        editor.map_rect.x + editor.map_rect.w + 16, editor.map_rect.y,
        editor.map->tss->texture->w, editor.map->tss->texture->h,
    };
    editor.ts_n = 0;
}

static void drawline(GLfloat size,
                     GLfloat xp1, GLfloat yp1, GLfloat xp2, GLfloat yp2) {
    GLfloat r = size / 2.0f;
    GLfloat xline = xp2 - xp1;
    GLfloat yline = yp2 - yp1;
    GLfloat length, rotation;

    length = sqrtf(xline * xline + yline * yline);
    rotation = atan2f(yline, xline);

    glLoadIdentity();
    glTranslatef(xp1, yp1, 0.0f);
    glRotatef(rotation / M_PI * 180.f, 0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(0, -r);
    glVertex2f(length, -r);
    glVertex2f(length, r);
    glVertex2f(0, r);
    glEnd();
}

static void draw_base_lines(void) {
    struct map *m = editor.map;
    int i, xmax, ymax;

    xmax = editor.map_rect.x + editor.map_rect.w;
    ymax = editor.map_rect.y + editor.map_rect.h;

    glColor3f(0.0f, 0.0f, 0.0f);
    for (i = 0; i <= m->row; ++i) {
        drawline(1.0f, editor.map_rect.x, editor.map_rect.y + i * m->th,
                 xmax, editor.map_rect.y + i * m->th);
    }
    for (i = 0; i <= m->col; ++i) {
        drawline(1.0f, editor.map_rect.x + i * m->tw, editor.map_rect.y,
                 editor.map_rect.x + i * m->tw, ymax);
    }
}

static int get_tileset_its(void) {
    int i;

    for (i = 0; i < editor.map->nts; ++i) {
        if (strcmp(editor.ts->pathname, editor.map->tss[i].pathname) == 0) {
            return i;
        }
    }

    return -1;
}

static void handle_lb_press(void) {
    if (sf_rect_iscontain(&editor.map_rect,
                          editor.mouse.lb.x, editor.mouse.lb.y)) {
        /* mouse click at map area */
        int col, row;

        col = (editor.mouse.lb.x - editor.map_rect.x) / editor.map->tw;
        row = (editor.mouse.lb.y - editor.map_rect.y) / editor.map->th;

        map_set_tile(editor.map, col, row, get_tileset_its(), editor.ts_n);
    } else if (sf_rect_iscontain(&editor.ts_rect,
                                 editor.mouse.lb.x, editor.mouse.lb.y)) {
        /* mouse click at tileset area */
        editor.ts_col = (editor.mouse.lb.x - editor.ts_rect.x) / editor.ts->tw;
        editor.ts_row = (editor.mouse.lb.y - editor.ts_rect.y) / editor.ts->th;
        editor.ts_n = editor.ts->col * editor.ts_row + editor.ts_col;
    }
}

static void update(void) {
    if (editor.mouse.lb.ispressed) {
        handle_lb_press();
    }
}

static void render(void) {
    draw_base_lines();
    map_draw(editor.map, editor.map_rect.x, editor.map_rect.y);
    sf_texture_draw(editor.map->tss->texture,
                    editor.ts_rect.x, editor.ts_rect.y);
    /* draw tileset hint */ {
        int x, y;

        x = editor.ts_rect.x + editor.ts_col * editor.ts->tw;
        y = editor.ts_rect.y + editor.ts_row * editor.ts->th;
        glColor3f(1.0f, 0.0f, 0.0f);
        drawline(1.0f, x, y, x + editor.ts->tw, y + editor.ts->th);
    }
}

int main(int argc, char *argv[]) {
    if (sf_init(argc, argv) != 0) {
        fprintf(stderr, "sf_init error\n");
        return -1;
    }

    editor_init();

    while (sf_window_isopen(editor.window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        update();
        render();

        glfwSwapBuffers(editor.window->handle);
        glfwPollEvents();
    }

    /*sf_uninit();*/

    return 0;
}
