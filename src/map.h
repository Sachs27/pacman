#ifndef MAP_H
#define MAP_H


#include "sf_texture.h"


#define MAP_NULL 0
#define MAP_SMALL_DOT 18
#define MAP_LARGE_DOT 9
#define MAP_DOOR      27


struct tileset {
    struct sf_texture *texture;
    char *pathname;
    int tw;     /* tile width(px) */
    int th;     /* tile height(px) */
    int col;    /* tileset width(tile) */
    int row;    /* tileset height(tile) */
};

struct map {
    char           *pathname;
    struct tileset *ts;

    int             tw;     /* tile width(px) */
    int             th;     /* tile height(px) */
    int             col;    /* map column(tile) */
    int             row;    /* map row(tile) */
    int             ntile;  /* == col * row */
    int            *tiles;
};



struct map *map_create(const char *pathname, int col, int row, int tw, int th);
struct map *map_load(const char *pathname);
void map_save(const struct map *m);
void map_set_tileset(struct map *m, const char *pathname, int tw, int th);
void map_set_tile(struct map *m, int col, int row, int n);
void map_draw(const struct map *m, int x, int y);


#endif /* MAP_H */
