#ifndef MAP_H
#define MAP_H


#include "sf_texture.h"


#define MAP_MAX_TILESETS 4

struct tileset {
    struct sf_texture *texture;
    char *pathname;
    int tw;     /* tile width(px) */
    int th;     /* tile height(px) */
    int col;    /* tileset width(tile) */
    int row;    /* tileset height(tile) */
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
    int             col;    /* map column(tile) */
    int             row;    /* map row(tile) */
    int             ntile;  /* == col * row */
    struct tile    *tiles;
};



struct map *map_create(const char *pathname, int col, int row, int tw, int th);
struct map *map_load(const char *pathname);
void map_save(const struct map *m);
void map_set_tileset(struct map *m, int its,
                     const char *pathname, int tw, int th);
void map_set_tile(struct map *m, int col, int row, int its, int n);
void map_draw(const struct map *m, int x, int y);


#endif /* MAP_H */
