#include "sf_frame.h"


void sf_frame_set(struct sf_frame *frame, struct sf_texture *texture,
                  int x, int y, int w, int h) {
    frame->texture = texture;
    frame->rect.x = x;
    frame->rect.y = y;
    frame->rect.w = w;
    frame->rect.h = h;
}

void sf_frame_draw(struct sf_frame *frame, GLfloat x, GLfloat y) {
    return sf_frame_draw_alpha(frame, x, y, 1.0f);
}

void sf_frame_draw_alpha(struct sf_frame *frame, GLfloat x, GLfloat y,
                         GLfloat alpha) {
    GLfloat top, bottom, left, right;

    if (frame->texture->tid == 0)
        return;

    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    glLoadIdentity();
    glTranslatef(x, y, 0.0f);

    left = frame->rect.x * 1.0f / frame->texture->w;
    right = (frame->rect.x + frame->rect.w) * 1.0f / frame->texture->w;
    top = frame->rect.y * 1.0f / frame->texture->h;
    bottom = (frame->rect.y + frame->rect.h) * 1.0f / frame->texture->h;

    glBindTexture(GL_TEXTURE_2D, frame->texture->tid);
    glBegin(GL_QUADS);
        glTexCoord2f(left, top); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(right, top); glVertex2f(frame->rect.w, 0.0f);
        glTexCoord2f(right, bottom); glVertex2f(frame->rect.w, frame->rect.h);
        glTexCoord2f(left, bottom); glVertex2f(0.0f, frame->rect.h);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
