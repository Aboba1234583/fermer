#ifndef MAP_H
#define MAP_H

#include "player.h"

#define MAP_MIN_SIZE 5
#define MAP_MAX_SIZE 30
#define TREE_CHANCE 15
#define STONE_CHANCE 15
#define CAMP '^'
#define PORTAL 'P'
#define PLAYER_SYM '@'
#define EMPTY '.'
#define TREE 'T'
#define STONE 'R'
#define BASE_GOLD_MULTIPLIER 10

typedef enum {
    BIOME_DEFAULT, BIOME_HEATH, BIOME_SWAMP, BIOME_RUINS, BIOME_CAVE
} Biome;

typedef struct {
    char **grid;
    int height;
    int width;
    int depth;
    Biome biome;
} Map;

Map* create_map(int depth);
void free_map(Map **m);
void generate_map(Map *m, Player *p);
void adventure_mode(Player **p_ptr, int *current_day, int *current_hour, char item_names[][MAX_NAME_LEN]);

#endif