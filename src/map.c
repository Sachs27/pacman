#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__UNIX__)
#include <linux/limits.h>
#endif


#include "sf_frame.h"
#include "map.h"


static struct tileset *tileset_load(const char *pathname, int tw, int th) {
    struct tileset *ts;

    ts = malloc(sizeof(*ts));
    ts->texture = sf_texture_load(pathname);
    ts->pathname = strdup(pathname);
    ts->tw = tw;
    ts->th = th;
    ts->col = ts->texture->w / ts->tw;
    ts->row = ts->texture->h / ts->th;

    return ts;
}

static void tileset_destroy(struct tileset *ts) {
    sf_texture_destroy(ts->texture);
    free(ts->pathname);
    free(ts);
}


struct map *map_create(const char *pathname, int col, int row,
                                             int tw, int th) {
    struct map *m;

    m = malloc(sizeof(*m));
    m->pathname = strdup(pathname);
    m->ts = NULL;
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
    int         len, tw, th;
    FILE       *f;
    struct map *m;

    m = malloc(sizeof(*m));
    f = fopen(pathname, "rb");
    /* load pathname */
    fread(&len, sizeof(int), 1, f);
    fread(buf, sizeof(char), len, f);
    m->pathname = strdup(buf);
    /* map properties */
    fread(&m->tw, sizeof(m->tw), 1, f);
    fread(&m->th, sizeof(m->th), 1, f);
    fread(&m->col, sizeof(m->col), 1, f);
    fread(&m->row, sizeof(m->row), 1, f);
    /* tilesets */
    fread(&len, sizeof(int), 1, f);
    fread(buf, sizeof(char), len, f);
    fread(&tw, sizeof(tw), 1, f);
    fread(&th, sizeof(th), 1, f);
    m->ts = tileset_load(buf, tw, th);
    /* tiles */
    fread(&m->ntile, sizeof(m->ntile), 1, f);
    m->tiles = calloc(m->ntile, sizeof(*m->tiles));
    fread(m->tiles, sizeof(*m->tiles), m->ntile, f);

    fclose(f);

    return m;
}

/*
 *  [int]       map name length(including null)
 *  [string]    map name
 *  [int]       map tile width
 *  [int]       map tile height
 *  [int]       map column
 *  [int]       map row
 *  [int]       tileset name length(including null)
 *  [string]    tileset name
 *  [int]       tileset tile width
 *  [int]       tileset tile height
 *  [int]       tile's number
 *  [int]       tiles
 *  ...
 */
void map_save(const struct map *m) {
    int     len;
    FILE   *f;

    f = fopen(m->pathname, "wb");
    /* map name */
    len = strlen(m->pathname) + 1;
    fwrite(&len, sizeof(len), 1, f);
    fwrite(m->pathname, sizeof(char), len, f);
    /* map properties */
    fwrite(&m->tw, sizeof(m->tw), 1, f);
    fwrite(&m->th, sizeof(m->th), 1, f);
    fwrite(&m->col, sizeof(m->col), 1, f);
    fwrite(&m->row, sizeof(m->row), 1, f);
    /* tileset */
    len = strlen(m->ts->pathname) + 1;
    fwrite(&len, sizeof(len), 1, f);
    fwrite(m->ts->pathname, sizeof(char), len, f);
    fwrite(&m->ts->tw, sizeof(m->ts->tw), 1, f);
    fwrite(&m->ts->th, sizeof(m->ts->th), 1, f);
    /* tiles */
    fwrite(&m->ntile, sizeof(m->ntile), 1, f);
    fwrite(m->tiles, sizeof(*m->tiles), m->ntile, f);

    fclose(f);
}

void map_set_tileset(struct map *m, const char *pathname, int tw, int th) {
    if (m->ts) {
        tileset_destroy(m->ts);
    }
    m->ts = tileset_load(pathname, tw, th);
}

void map_set_tile(struct map *m, int col, int row, int n) {
    int i;

    i = row * m->col + col;
    if (i >= m->ntile) {
        return;
    }
    m->tiles[i] = n;
}

void map_draw(const struct map *m, int x, int y) {
    int col, row;
    int *tile;

    if (m->ts == NULL) {
        return;
    }

    tile = m->tiles;
    for (row = 0; row < m->row; ++row)
    for (col = 0; col < m->col; ++col, ++tile) {
        struct sf_frame f;
        int n = *tile;

        f.texture = m->ts->texture;
        f.rect.x = (n % m->ts->col) * m->ts->tw;
        f.rect.y = (n / m->ts->col) * m->ts->th;
        f.rect.w = m->ts->tw;
        f.rect.h = m->ts->th;

        sf_frame_draw(&f, x + col * m->tw, y + row * m->th);
    }
}
