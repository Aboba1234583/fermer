#include "player.h"

Player* create_player(void) {
    Player *p = (Player*)malloc(sizeof(Player));
    if (p == NULL) { printf("Ошибка памяти!\n"); exit(1); }
    get_string_input("Введите имя фермера: ", p->name, sizeof(p->name));
    if (strlen(p->name) == 0) strcpy(p->name, "Безымянный");
    
    p->max_hp = 100; p->hp = 100;
    p->max_stamina = 50; p->stamina = 50;
    p->gold = 0;
    p->x = 0; p->y = 0;
    
    int default_inv[INVENTORY_SIZE] = {4, 6, 0, 2, 0, 3, 8, 0, 1, 0};
    for (int i = 0; i < INVENTORY_SIZE; i++) p->inventory[i] = default_inv[i];
    
    printf("Создан персонаж: %s (Адрес: %p)\n", p->name, (void*)p);
    return p;
}

void free_player(Player **p) {
    if (p != NULL && *p != NULL) {
        printf("Память структуры по адресу %p освобождена\n", (void*)*p);
        free(*p);
        *p = NULL;
    }
}

void print_player_stats(const Player *p, int day, int hour, char item_names[][MAX_NAME_LEN]) {
    if (p == NULL) { printf("Персонаж не создан!\n"); return; }
    printf("\n=== Карточка ===\nИмя: %s\nHP: %d/%d | Выносливость: %d/%d | Золото: %d\nВремя: День %d, %02d:00\n", 
           p->name, p->hp, p->max_hp, p->stamina, p->max_stamina, p->gold, day, hour);
    print_inventory(p->inventory, item_names, INVENTORY_SIZE);
}