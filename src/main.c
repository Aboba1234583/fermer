#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "player.h"
#include "map.h"

int main(void) {
    srand(time(NULL));
    char item_names[10][MAX_NAME_LEN];
    load_item_names("items.txt", item_names);
    
    Player *player = NULL;
    int current_day = 1, current_hour = 8, choice;

    while (1) {
        printf("\n=== Меню ===\n");
        if (player != NULL) printf("Игрок: %s | Золото: %d | Силы: %d\n", player->name, player->gold, player->stamina);
        printf("[0] Выход\n[1] Создать/пересоздать игрока\n[2] Карточка\n[3] Часы\n[4] Ждать\n[5] Спать\n[6] Инвентарь\n[7] Поиск\n[8] Дневник\n[9] Рецепты\n[10] В приключение\n");

        choice = get_int_input("Выбор: ", 0, 10);

        switch (choice) {
            case 0: free_player(&player); printf("Выход.\n"); return 0;
            case 1:
                if (player != NULL) {
                    char confirm[MAX_NAME_LEN];
                    get_string_input("Игрок есть. Удалить? (y/n): ", confirm, sizeof(confirm));
                    if (confirm[0] == 'y' || confirm[0] == 'Y') { free_player(&player); player = create_player(); }
                } else player = create_player();
                break;
            case 2: print_player_stats(player, current_day, current_hour, item_names); break;
            case 3: print_clock(current_day, current_hour); break;
            case 4: {
                int hours = get_int_input("Сколько часов ждать? ", 1, 100);
                int new_hour = current_hour + hours;
                if (new_hour >= 2 && new_hour < 8) printf("Нельзя проматывать через ночь!\n");
                else { current_hour += hours; while (current_hour >= 24) { current_hour -= 24; current_day++; } }
                break;
            }
            case 5:
                current_day++; current_hour = 8;
                if (player != NULL) { player->hp = player->max_hp; player->stamina = player->max_stamina; printf("Отдохнули.\n"); }
                break;
            case 6:
                if (player == NULL) printf("Сначала создайте игрока!\n");
                else print_inventory(player->inventory, item_names, INVENTORY_SIZE);
                break;
            case 7: 
                if (player == NULL) printf("Сначала создайте игрока!\n");
                else { char s[MAX_NAME_LEN]; get_string_input("Название: ", s, sizeof(s)); printf("Поиск выполнен.\n"); }
                break;
            case 8:
                if (player == NULL) printf("Сначала создайте игрока!\n");
                else { char d[MAX_BUFFER]; snprintf(d, sizeof(d), "День: %d, Время: %02d:00, Золото: %d\n", current_day, current_hour, player->gold); write_diary_to_file("diary.txt", d); }
                break;
            case 9: printf("Проверка рецептов (заглушка).\n"); break;
            case 10: adventure_mode(&player, &current_day, &current_hour, item_names); break;
        }
    }
    return 0;
}