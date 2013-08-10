#ifndef MAP_H
#define MAP_H


#include "sf_texture.h"


#define MAP_MAX_TILESETS 4

struct tileset {
    struct sf_texture *tex;
    char *pathname;
    int tw;     /* tile width(px) */
    int th;     /* tile height(px) */
    int w;      /* tileset width(tile) */
    int h;      /* tileset height(tile) */
};

struct tile {
    int             its;
    struct tileset *ts;
    int             n;
};

struct map {
    char           *pathname;
    int             nts;
    struct tileset *tss;

    int             tw;     /* tile width(px) */
    int             th;     /* tile height(px) */
    int             w;      /* map width(tile) */
    int             h;      /* map height(tile) */
    int             ntile;  /* == w * h */
    struct tile    *tiles;
};



struct map *map_create(const char *pathname, int w, int h, int tw, int th);
struct map *map_load(const char *pathname);
void map_save(const struct map *m);
void map_set_tileset(struct map *m, int its,
                     const char *pathname, int tw, int th);
void map_set_tile(struct map *m, int x, int y, int its, int n);
void map_draw(const struct map *m);


#endif /* MAP_H */
