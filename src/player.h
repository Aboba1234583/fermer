#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

#define INVENTORY_SIZE 10

typedef struct {
    char name[MAX_NAME_LEN];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x;
    int y;
    int inventory[INVENTORY_SIZE];
} Player;

Player* create_player(void);
void free_player(Player **p);
void print_player_stats(const Player *p, int day, int hour, char item_names[][MAX_NAME_LEN]);

#endif