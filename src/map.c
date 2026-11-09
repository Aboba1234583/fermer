#include "map.h"
#include <time.h>

Map* create_map(int depth) {
    Map *m = (Map*)malloc(sizeof(Map));
    if (m == NULL) exit(1);
    m->depth = depth;
    m->height = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);
    m->width = MAP_MIN_SIZE + rand() % (MAP_MAX_SIZE - MAP_MIN_SIZE + 1);
    
    m->grid = (char**)malloc(m->height * sizeof(char*));
    for (int i = 0; i < m->height; i++) {
        m->grid[i] = (char*)malloc(m->width * sizeof(char));
    }
    
    int r = rand() % 100;
    if (r < 25) m->biome = BIOME_HEATH;
    else if (r < 50) m->biome = BIOME_SWAMP;
    else if (r < 75) m->biome = BIOME_RUINS;
    else m->biome = BIOME_CAVE;
    
    return m;
}

void free_map(Map **m) {
    if (m != NULL && *m != NULL) {
        for (int i = 0; i < (*m)->height; i++) free((*m)->grid[i]);
        free((*m)->grid);
        printf("Память карты по адресу %p освобождена\n", (void*)*m);
        free(*m);
        *m = NULL;
    }
}

void generate_map(Map *m, Player *p) {
    int tree_chance = TREE_CHANCE;
    int stone_chance = STONE_CHANCE;
    
    if (m->biome == BIOME_HEATH) tree_chance += 10;
    else if (m->biome == BIOME_RUINS) stone_chance += 10;
    
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            int r = rand() % 100;
            if (r < tree_chance) m->grid[i][j] = TREE;
            else if (r < tree_chance + stone_chance) m->grid[i][j] = STONE;
            else m->grid[i][j] = EMPTY;
        }
    }
    
    int f_placed = 0;
    while (!f_placed) {
        int ty = rand() % m->height;
        int tx = rand() % m->width;
        if (m->grid[ty][tx] == EMPTY) { m->grid[ty][tx] = CAMP; f_placed = 1; }
    }
    
    int h_placed = 0;
    while (!h_placed) {
        int ty = rand() % m->height;
        int tx = rand() % m->width;
        if (m->grid[ty][tx] == EMPTY) { p->y = ty; p->x = tx; h_placed = 1; }
    }
}

void adventure_mode(Player **p_ptr, int *current_day, int *current_hour, char item_names[][MAX_NAME_LEN]) {
    Player *p = *p_ptr;
    if (p == NULL) { printf("Сначала создайте персонажа!\n"); return; }
    if (p->stamina <= 0) { printf("Нет сил для приключений!\n"); return; }
    if (*current_hour >= 2 && *current_hour < 8) { printf("Ночью нужно спать!\n"); return; }

    Map *m = create_map(1);
    generate_map(m, p);
    char action;
    char cell_under = m->grid[p->y][p->x];
    int edge_crosses = 0, portal_active = 0;

    while (1) {
        printf("\n=== Лес (Этаж: %d) ===\n", m->depth);
        if (m->biome == BIOME_HEATH) printf("Биом: Редколесье\n");
        else if (m->biome == BIOME_SWAMP) printf("Биом: Топь\n");
        else if (m->biome == BIOME_RUINS) printf("Биом: Каменные руины\n");
        else if (m->biome == BIOME_CAVE) printf("Биом: Кристальная пещера\n");
        printf("HP: %d/%d | Выносливость: %d/%d | Золото: %d\n\n", p->hp, p->max_hp, p->stamina, p->max_stamina, p->gold);
        
        for (int i = 0; i < m->height; i++) {
            for (int j = 0; j < m->width; j++) {
                if (i == p->y && j == p->x) printf("%c", PLAYER_SYM);
                else printf("%c", m->grid[i][j]);
            }
            printf("\n");
        }

        printf("Ход (W/A/S/D, Q-выход, M-карта): ");
        scanf(" %c", &action);
        clear_input_buffer();

        if (action == 'q' || action == 'Q') {
            if (cell_under == CAMP) { printf("Выход на ферму.\n"); break; }
            else { printf("Выход только у костра (^)!\n"); continue; }
        }

        if (action == 'm' || action == 'M') {
            if (p->stamina >= 5) {
                p->stamina -= 5;
                p->gold += 20;
                printf("Карта зарисована! +20 золота.\n");
            } else printf("Не хватает сил на карту!\n");
            continue;
        }

        int new_y = p->y, new_x = p->x;
        if (action == 'w' || action == 'W') new_y--;
        else if (action == 's' || action == 'S') new_y++;
        else if (action == 'a' || action == 'A') new_x--;
        else if (action == 'd' || action == 'D') new_x++;
        else continue;

        int crossed = 0;
        if (new_y < 0 || new_y >= m->height || new_x < 0 || new_x >= m->width) crossed = 1;
        new_y = (new_y % m->height + m->height) % m->height;
        new_x = (new_x % m->width + m->width) % m->width;

        if (m->grid[new_y][new_x] == TREE || m->grid[new_y][new_x] == STONE) {
            printf("Путь заблокирован.\n"); continue;
        }

        p->stamina--;
        if (p->stamina <= 0) {
            printf("Вы устали! Эвакуация...\n");
            p->gold /= 2;
            break;
        }

        m->grid[p->y][p->x] = cell_under;
        cell_under = m->grid[new_y][new_x];
        p->y = new_y; p->x = new_x;

        if (cell_under == PORTAL) {
            printf("Переход на новый этаж!\n");
            (*current_hour)++;
            p->stamina -= 5;
            if (p->stamina <= 0) { printf("Обессилели! Эвакуация...\n"); p->gold /= 2; break; }
            
            int reward = BASE_GOLD_MULTIPLIER * m->depth;
            if (m->biome == BIOME_CAVE) reward *= 3;
            p->gold += reward;
            printf("Награда: %d золота.\n", reward);

            free_map(&m);
            m = create_map(m->depth + 1);
            generate_map(m, p);
            cell_under = m->grid[p->y][p->x];
            edge_crosses = 0; portal_active = 0;
        }

        m->grid[p->y][p->x] = PLAYER_SYM;

        if (crossed) {
            edge_crosses++;
            if (edge_crosses >= 3 && !portal_active) {
                portal_active = 1;
                printf("Появился портал (P)!\n");
                int p_placed = 0;
                while (!p_placed) {
                    int ty = rand() % m->height, tx = rand() % m->width;
                    if (m->grid[ty][tx] == EMPTY) { m->grid[ty][tx] = PORTAL; p_placed = 1; }
                }
            }
        }
    }
    free_map(&m);
}