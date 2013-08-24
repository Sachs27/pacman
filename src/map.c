#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include "sf_frame.h"
#include "map.h"


static void tile_draw(struct tile *tile, int x, int y) {
    struct sf_frame f;
    int col, row;

    if (tile->ts->texture == NULL) {
        return;
    }

    col = tile->n % tile->ts->col;
    row = tile->n / tile->ts->col;

    f.texture = tile->ts->texture;
    f.rect.x = col * tile->ts->tw;
    f.rect.y = row * tile->ts->th;
    f.rect.w = tile->ts->tw;
    f.rect.h = tile->ts->th;

    sf_frame_draw(&f, (float) x, (float) y);
}


static void tileset_load(struct tileset *ts,
                         const char *pathname, int tw, int th) {
    ts->texture = sf_texture_load(pathname);
    ts->pathname = strdup(pathname);
    ts->tw = tw;
    ts->th = th;
    ts->col = ts->texture->w / ts->tw;
    ts->row = ts->texture->h / ts->th;
}


struct map *map_create(const char *pathname, int col, int row,
                                             int tw, int th) {
    struct map *m;

    m = malloc(sizeof(*m));
    m->pathname = strdup(pathname);
    m->nts = 0;
    m->tss = calloc(MAP_MAX_TILESETS, sizeof(*m->tss));
    m->col = col;
    m->row = row;
    m->tw = tw;
    m->th = th;
    m->ntile = col * row;
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
    m->pathname = strdup(buf);
    /* map properties */
    fread(&m->tw, sizeof(m->tw), 1, f);
    fread(&m->th, sizeof(m->th), 1, f);
    fread(&m->col, sizeof(m->col), 1, f);
    fread(&m->row, sizeof(m->row), 1, f);
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
    fwrite(&m->col, sizeof(m->col), 1, f);
    fwrite(&m->row, sizeof(m->row), 1, f);
    /* tilesets */
    fwrite(&m->nts, sizeof(m->nts), 1, f);
    for (i = 0; i < m->nts; ++i) {
        if (m->tss[i].texture == NULL) {
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

void map_set_tile(struct map *m, int col, int row, int its, int n) {
    int i;
    struct tile *tile;

    i = row * m->col + col;
    if (i >= m->ntile) {
        return;
    }
    tile = m->tiles + i;
    tile->its = its;
    tile->ts = m->tss + its;
    tile->n = n;
}

void map_draw(const struct map *m, int x, int y) {
    int col, row;
    struct tile *tile;

    tile = m->tiles;
    for (row = 0; row < m->row; ++row) {
        for (col = 0; col < m->col; ++col, ++tile) {
            if (tile->ts != NULL) {
                tile_draw(tile, x + col * m->tw, y + row * m->th);
            }
        }
    }
}
