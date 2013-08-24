#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include <GLFW/glfw3.h>

#include "sf.h"
#include "sf_frame.h"
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

static void map_window_on_render(struct ui *w);
static void map_window_on_click(struct ui *w, int x, int y);

static void ts_window_on_render(struct ui *w);
static void ts_window_on_click(struct ui *w, int x, int y);

static void editor_init(void) {
    editor.window = sf_window_create("Pacman - Map Editor",
                                     EDITOR_WINDOW_W, EDITOR_WINDOW_H);
    glfwSetMouseButtonCallback(editor.window->handle, on_mouse_button);
    glfwSetCursorPosCallback(editor.window->handle, on_mouse_pos);
    glfwSetWindowSizeCallback(editor.window->handle, on_window_size);

    init_gl();

    editor.bt_create = ui_create(0, 0, 40, 40);
    ui_set_background(editor.bt_create, sf_texture_load("data/textures/create.png"));

    editor.bt_destroy = ui_create(40, 0, 40, 40);
    ui_set_background(editor.bt_destroy, sf_texture_load("data/textures/destroy.png"));

    editor.bt_prev = ui_create(80, 0, 40, 40);
    ui_set_background(editor.bt_prev, sf_texture_load("data/textures/prev.png"));

    editor.bt_next = ui_create(120, 0, 40, 40);
    ui_set_background(editor.bt_next, sf_texture_load("data/textures/next.png"));

    editor.bt_save = ui_create(160, 0, 40, 40);
    ui_set_background(editor.bt_save, sf_texture_load("data/textures/save.png"));

    editor.map = map_create("data/maps/classic",
                            PACMAN_MAP_COL, PACMAN_MAP_ROW,
                            PACMAN_MAP_TW, PACMAN_MAP_TH);
    map_set_tileset(editor.map, 0, "data/textures/walls.png", 16, 16);

    editor.map_window = ui_create(1, 40, editor.map->col * editor.map->tw,
                                  editor.map->row * editor.map->th);
    ui_on_render(editor.map_window, map_window_on_render);
    ui_on_click(editor.map_window, map_window_on_click);

    editor.ts = editor.map->tss;
    editor.ts_window = ui_create(editor.map_window->area.x
                                 + editor.map_window->area.w + 16,
                                 editor.map_window->area.y,
                                 editor.map->tss->texture->w,
                                 editor.map->tss->texture->h);
    ui_set_background(editor.ts_window, editor.map->tss->texture);
    ui_on_render(editor.ts_window, ts_window_on_render);
    ui_on_click(editor.ts_window, ts_window_on_click);
    editor.ts_col = 0;
    editor.ts_row = 0;
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

    glPushMatrix();
    glTranslatef(xp1, yp1, 0.0f);
    glRotatef(rotation / M_PI * 180.f, 0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(0, -r);
    glVertex2f(length, -r);
    glVertex2f(length, r);
    glVertex2f(0, r);
    glEnd();

    glPopMatrix();
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

static void draw_base_lines(struct ui *w) {
    struct map *m = editor.map;
    int i, xmax, ymax;

    xmax = w->area.w;
    ymax = w->area.h;

    glColor3f(0.0f, 0.0f, 0.0f);
    for (i = 0; i <= m->row; ++i) {
        drawline(1.0f, 0.0f, i * m->th, xmax, i * m->th);
    }
    for (i = 0; i <= m->col; ++i) {
        drawline(1.0f, i * m->tw, 0.0f, i * m->tw, ymax);
    }
}

static void map_window_on_render(struct ui *w) {
    draw_base_lines(w);
    map_draw(editor.map, 0.0f, 0.0f);
}

static void map_window_on_click(struct ui *w, int x, int y) {
    int col, row;

    col = x / editor.map->tw;
    row = y / editor.map->th;

    map_set_tile(editor.map, col, row, get_tileset_its(), editor.ts_n);
}

static void ts_window_on_render(struct ui *w) {
    /* draw tileset hint */ {
        int x, y;

        x = editor.ts_col * editor.ts->tw;
        y = editor.ts_row * editor.ts->th;
        glColor3f(1.0f, 0.0f, 0.0f);
        drawline(1.0f, x, y, x + editor.ts->tw, y + editor.ts->th);
        drawline(1.0f, x + editor.ts->tw, y, x, y + editor.ts->th);
    }
}

static void ts_window_on_click(struct ui *w, int x, int y) {
    editor.ts_col = x / editor.ts->tw;
    editor.ts_row = y / editor.ts->th;
    editor.ts_n = editor.ts->col * editor.ts_row + editor.ts_col;
}

static void handle_lb_press(int x, int y) {
    struct ui *clicked_window = NULL;

    if (sf_rect_iscontain(&editor.map_window->area, x, y)) {
        /* mouse click at map area */
        clicked_window = editor.map_window;
    } else if (sf_rect_iscontain(&editor.ts_window->area, x, y)) {
        /* mouse click at tileset area */
        clicked_window = editor.ts_window;
    }

    if (clicked_window && clicked_window->on_click) {
        clicked_window->on_click(clicked_window,
                                 x - clicked_window->area.x,
                                 y - clicked_window->area.y);
    }

}

static void ui_update(void) {
    static int ui_lb_ispressed = 0;

    if (editor.mouse.lb.ispressed && !ui_lb_ispressed) {
        ui_lb_ispressed = 1;
        handle_lb_press(editor.mouse.lb.x, editor.mouse.lb.y);
    } else if (!editor.mouse.lb.ispressed) {
        ui_lb_ispressed = 0;
    }
}

static void update(void) {
    ui_update();
}

static void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    ui_render(editor.map_window);
    ui_render(editor.ts_window);
    ui_render(editor.bt_create);
    ui_render(editor.bt_destroy);
    ui_render(editor.bt_prev);
    ui_render(editor.bt_next);
    ui_render(editor.bt_save);
}

int main(int argc, char *argv[]) {
    if (sf_init(argc, argv) != 0) {
        fprintf(stderr, "sf_init error\n");
        return -1;
    }

    editor_init();

    while (sf_window_isopen(editor.window)) {
        update();
        render();

        glfwSwapBuffers(editor.window->handle);
        glfwPollEvents();
    }

    /*sf_uninit();*/

    return 0;
}
