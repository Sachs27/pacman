#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <IL/il.h>

#include "sf_texture.h"


struct texture_inner {
    ILuint il_id;
    GLsizei width;
    GLsizei height;
    GLenum format;
    GLenum type;
    const GLvoid *pixels;
};


static int texture_inner_init(struct texture_inner *tex_inner,
                              const char *pathname) {
    ilGenImages(1, &tex_inner->il_id);
    ilBindImage(tex_inner->il_id);

    if (ilLoadImage(pathname) != IL_TRUE) {
        fprintf(stderr, "Failed to load texture: %s.\n", pathname);
        goto TEX_INIT_FAILED;
    }

    if (ilConvertImage(IL_RGBA, IL_FLOAT) != IL_TRUE)
        goto TEX_INIT_FAILED;

    goto TEX_INIT_OK;

TEX_INIT_FAILED:
    ilDeleteImages(1, &tex_inner->il_id);
    ilBindImage(0);
    return -1;


TEX_INIT_OK:
    tex_inner->pixels = ilGetData();
    tex_inner->width = ilGetInteger(IL_IMAGE_WIDTH);
    tex_inner->height = ilGetInteger(IL_IMAGE_HEIGHT);
    tex_inner->format = IL_RGBA;
    tex_inner->type = IL_FLOAT;

    ilBindImage(0);

    return 0;
}

static void texture_inner_uninit(struct texture_inner *tex_inner) {
    ilDeleteImages(1, &tex_inner->il_id);
}

struct sf_texture *sf_texture_load(const char *pathname) {
    struct sf_texture *tex;
    struct texture_inner tex_inner;

    tex = malloc(sizeof(*tex));
    if (tex == NULL)
        return NULL;
    tex->pathname = malloc(strlen(pathname) + 1);
    strcpy(tex->pathname, pathname);

    if (texture_inner_init(&tex_inner, pathname) != 0) {
        free(tex);
        return NULL;
    }

    tex->w = tex_inner.width;
    tex->h = tex_inner.height;
    glGenTextures(1, &tex->tid);

    glBindTexture(GL_TEXTURE_2D, tex->tid);
    glTexImage2D(GL_TEXTURE_2D, 0, tex_inner.format,
            tex_inner.width, tex_inner.height, 0,
            tex_inner.format, tex_inner.type, tex_inner.pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    texture_inner_uninit(&tex_inner);

    if (glGetError() != GL_NO_ERROR) {
        free(tex);
        return NULL;
    }

    return tex;
}

void sf_texture_destroy(struct sf_texture *tex) {
    glDeleteTextures(1, &tex->tid);
    free(tex->pathname);
    free(tex);
}

void sf_texture_draw(const struct sf_texture *tex, GLfloat x, GLfloat y) {
    return sf_texture_draw_alpha(tex, x, y, 1.0f);
}

void sf_texture_draw_alpha(const struct sf_texture *tex,
                           GLfloat x, GLfloat y, GLfloat alpha) {
    if (tex->tid == 0)
        return;

    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glBindTexture(GL_TEXTURE_2D, tex->tid);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(tex->w, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(tex->w, tex->h);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, tex->h);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

