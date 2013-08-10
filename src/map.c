#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_frame.h"
#include "map.h"


static void tile_draw(struct tile *tile, int x, int y) {
    struct sf_frame f;
    int w, h;

    w = tile->n % tile->ts->w;
    h = tile->n / tile->ts->h;

    f.texture = tile->ts->tex;
    f.rect.x = w * tile->ts->tw;
    f.rect.y = h * tile->ts->th;
    f.rect.w = tile->ts->tw;
    f.rect.h = tile->ts->th;

    sf_frame_draw(&f, (float) x, (float) y);
}


static void tileset_load(struct tileset *ts,
                         const char *pathname, int tw, int th) {
    ts->tex = sf_texture_load(pathname);
    ts->pathname = malloc(strlen(pathname) + 1);
    strcpy(ts->pathname, pathname);
    ts->tw = tw;
    ts->th = th;
    ts->w = ts->tex->w / ts->tw;
    ts->h = ts->tex->h / ts->th;
}


struct map *map_create(const char *pathname, int w, int h, int tw, int th) {
    struct map *m;

    m = malloc(sizeof(*m));
    m->pathname = malloc(strlen(pathname) + 1);
    strcpy(m->pathname, pathname);
    m->nts = 0;
    m->tss = calloc(MAP_MAX_TILESETS, sizeof(*m->tss));
    m->w = w;
    m->h = h;
    m->tw = tw;
    m->th = th;
    m->ntile = w * h;
    m->tiles = calloc(m->ntile, sizeof(*m->tiles));

    return m;
}

struct map *map_load(const char *pathname) {
    char        buf[PATH_MAX];
    char       *ptr;
    int         i;
    FILE       *f;
    struct map *m;

    m = malloc(sizeof(*m));
    f = fopen(pathname, "rb");
    /* load pathname */
    ptr = buf;
    fread(ptr, sizeof(char), 1, f);
    while (*ptr) {
        fread(++ptr, sizeof(char), 1, f);
    }
    m->pathname = malloc(strlen(buf) + 1);
    strcpy(m->pathname, buf);
    /* map properties */
    fread(&m->tw, sizeof(m->tw), 1, f);
    fread(&m->th, sizeof(m->th), 1, f);
    fread(&m->w, sizeof(m->w), 1, f);
    fread(&m->h, sizeof(m->h), 1, f);
    /* tilesets */
    fread(&m->nts, sizeof(m->nts), 1, f);
    m->tss = calloc(m->nts, sizeof(*m->tss));
    for (i = 0; i< m->nts; ++i) {
        int tw, th;

        ptr = buf;
        fread(ptr, sizeof(char), 1, f);
        while (*ptr) {
            fread(++ptr, sizeof(char), 1, f);
        }
        fread(&tw, sizeof(tw), 1, f);
        fread(&th, sizeof(th), 1, f);
        tileset_load(m->tss + i, buf, tw, th);
    }
    /* tiles */
    fread(&m->ntile, sizeof(m->ntile), 1, f);
    m->tiles = calloc(m->ntile, sizeof(*m->tiles));
    for (i = 0; i < m->ntile; ++i) {
        fread(&m->tiles[i].its, sizeof(&m->tiles[i].its), 1, f);
        fread(&m->tiles[i].n, sizeof(&m->tiles[i].n), 1, f);
        if (m->tiles[i].its >= 0) {
            m->tiles[i].ts = m->tss + m->tiles[i].its;
        }
    }

    fclose(f);

    return m;
}

void map_save(const struct map *m) {
    int     i;
    FILE   *f;


    f = fopen(m->pathname, "wb+");
    /* map name */
    fwrite(m->pathname, sizeof(char), strlen(m->pathname) + 1, f);
    /* map properties */
    fwrite(&m->tw, sizeof(m->tw), 1, f);
    fwrite(&m->th, sizeof(m->th), 1, f);
    fwrite(&m->w, sizeof(m->w), 1, f);
    fwrite(&m->h, sizeof(m->h), 1, f);
    /* tilesets */
    fwrite(&m->nts, sizeof(m->nts), 1, f);
    for (i = 0; i < m->nts; ++i) {
        if (m->tss[i].tex == NULL) {
            continue;
        }
        fwrite(m->tss[i].pathname, sizeof(char),
               strlen(m->tss[i].pathname) + 1, f);
        fwrite(&m->tss[i].tw, sizeof(m->tss[i].tw), 1, f);
        fwrite(&m->tss[i].th, sizeof(m->tss[i].th), 1, f);
    }
    /* tiles */
    fwrite(&m->ntile, sizeof(m->ntile), 1, f);
    for (i = 0; i < m->ntile; ++i) {
        if (m->tiles[i].ts == NULL) {
            m->tiles[i].its = -1;
            m->tiles[i].n = -1;
        }
        fwrite(&m->tiles[i].its, sizeof(&m->tiles[i].its), 1, f);
        fwrite(&m->tiles[i].n, sizeof(&m->tiles[i].n), 1, f);
    }

    fclose(f);
}

void map_set_tileset(struct map *m, int its,
                     const char *pathname, int tw, int th) {
    if (its < 0 || its >= MAP_MAX_TILESETS) {
        return;
    }
    ++m->nts;
    return tileset_load(m->tss + its, pathname, tw, th);
}

void map_set_tile(struct map *m, int x, int y, int its, int n) {
    struct tile *tile;

    tile = m->tiles + (y + x * m->w);
    tile->its = its;
    tile->ts = m->tss + its;
    tile->n = n;
}

void map_draw(const struct map *m) {
    int w, h;
    struct tile *tile;

    tile = m->tiles;
    for (h = 0; h < m->h; ++h) {
        for (w = 0; w < m->w; ++w, ++tile) {
            if (tile->ts != NULL) {
                tile_draw(tile, w * m->tw, h * m->th);
            }
        }
    }
}
