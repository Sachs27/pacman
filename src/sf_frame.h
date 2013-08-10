#ifndef SF_FRAME_H
#define SF_FRAME_H


#include "sf_rect.h"
#include "sf_texture.h"


struct sf_frame {
    struct sf_texture  *texture;
    struct sf_rect      rect;
};


void sf_frame_set(struct sf_frame *frame, struct sf_texture *tex,
                  int x, int y, int w, int h);
void sf_frame_draw(struct sf_frame *frame, GLfloat x, GLfloat y);
void sf_frame_draw_alpha(struct sf_frame *frame, GLfloat x, GLfloat y,
                         GLfloat alpha);


#endif /* SF_FRAME_H */
