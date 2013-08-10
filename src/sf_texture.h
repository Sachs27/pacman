#ifndef SF_TEXTURE_H
#define SF_TEXTURE_H


#include <GL/gl.h>


struct sf_texture {
    GLuint  tid;

    char   *pathname;
    int     w;
    int     h;
};


struct sf_texture *sf_texture_load(const char *pathname);
void sf_texture_destroy(struct sf_texture *tex);
void sf_texture_draw(const struct sf_texture *tex, GLfloat x, GLfloat y);
void sf_texture_draw_alpha(const struct sf_texture *tex,
                      GLfloat x, GLfloat y, GLfloat alpha);


#endif /* SF_TEXTURE_H */
