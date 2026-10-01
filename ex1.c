#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Константы (убраны магические числа) ---
#define INVENTORY_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
#define MAX_FARMER_NAME 32
#define MAX_BUFFER 512

#define FILENAME_ITEMS "items.txt"
#define FILENAME_DIARY "diary.txt"
#define FILENAME_INPUT "input.txt"
#define FILENAME_OUTPUT "output.txt"

#define EMPTY_SLOT_ID 0

// --- Прототипы функций ---

// Утилиты ввода/вывода
void clear_input_buffer(void);
int get_int_input(const char *prompt, int min, int max);
void get_string_input(const char *prompt, char *buffer, int max_len);

// Работа с данными (БЕЗ printf и scanf)
void load_item_names(const char *filename, char item_names[][MAX_NAME_LEN]);
void init_inventory(int inventory[], int size);
int find_item_id_by_name(const char *name, char item_names[][MAX_NAME_LEN], int max_items);
int calculate_favorite_resource(const int inventory[], int size);
void format_diary_entry(char *buffer, int buffer_size, const char *farmer_name, 
                        int day, int hour, const int inventory[], 
                        char item_names[][MAX_NAME_LEN], int inv_size);
void process_npc_template(const char *input_text, char *output_text, int out_size, 
                          const char *farmer_name, int day, 
                          const int inventory[], char item_names[][MAX_NAME_LEN]);

// Вывод информации (ТОЛЬКО printf)
void print_clock(int day, int hour);
void print_inventory(const int inventory[], char item_names[][MAX_NAME_LEN], int size);
void print_search_result(const char *search_name, int item_id, 
                         const int inventory[], int size, 
                         char item_names[][MAX_NAME_LEN]);
void print_favorite_resource(int fav_id, const int inventory[], 
                             char item_names[][MAX_NAME_LEN], int size);

// Обработка файлов
void load_data_from_files(char item_names[][MAX_NAME_LEN], int inventory[]);
void write_diary_to_file(const char *filename, const char *text);
void process_variant_4_to_file(const char *in_filename, const char *out_filename, 
                               const char *farmer_name, int day, 
                               const int inventory[], char item_names[][MAX_NAME_LEN]);

// --- Реализация утилит ввода ---

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_int_input(const char *prompt, int min, int max) {
    int value;
    while (1) {
        if (prompt && strlen(prompt) > 0) printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            if (value >= min && value <= max) {
                clear_input_buffer();
                return value;
            } else {
                printf("Ошибка: число должно быть от %d до %d.\n", min, max);
            }
        } else {
            printf("Ошибка: неверный формат ввода. Введите число.\n");
            clear_input_buffer();
        }
    }
}

void get_string_input(const char *prompt, char *buffer, int max_len) {
    printf("%s", prompt);
    if (fgets(buffer, max_len, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Удаляем \n
    } else {
        buffer[0] = '\0';
    }
}

// --- Реализация работы с данными (Логика) ---

void init_inventory(int inventory[], int size) {
    int default_inv[INVENTORY_SIZE] = {4, 6, 0, 2, 0, 3, 8, 0, 1, 0};
    for (int i = 0; i < size; i++) {
        inventory[i] = default_inv[i];
    }
}

void load_item_names(const char *filename, char item_names[][MAX_NAME_LEN]) {
    // Значения по умолчанию
    const char *default_names[MAX_ITEMS] = {
        "Пусто", "Дерево", "Камень", "Семена", "Лопата",
        "Грабли", "Тяпка", "Телега", "Лейка", "Корзина"
    };
    for (int i = 0; i < MAX_ITEMS; i++) {
        strncpy(item_names[i], default_names[i], MAX_NAME_LEN - 1);
        item_names[i][MAX_NAME_LEN - 1] = '\0';
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("[Предупреждение] Файл %s не найден. Используются стандартные названия.\n", filename);
        return;
    }

    char line[MAX_NAME_LEN];
    int id;
    while (fscanf(file, "%d %31s", &id, line) == 2) {
        if (id >= 0 && id < MAX_ITEMS) {
            strncpy(item_names[id], line, MAX_NAME_LEN - 1);
            item_names[id][MAX_NAME_LEN - 1] = '\0';
        }
    }
    fclose(file);
    printf("Каталог предметов загружен из %s.\n", filename);
}

int find_item_id_by_name(const char *name, char item_names[][MAX_NAME_LEN], int max_items) {
    for (int i = 0; i < max_items; i++) {
        if (strcmp(item_names[i], name) == 0) {
            return i;
        }
    }
    return -1; // Не найдено
}

int calculate_favorite_resource(const int inventory[], int size) {
    int max_count = 0;
    int favorite_id = -1;

    for (int i = 0; i < size; i++) {
        if (inventory[i] == EMPTY_SLOT_ID) continue;

        int count = 0;
        for (int j = 0; j < size; j++) {
            if (inventory[j] == inventory[i]) count++;
        }

        if (count > max_count) {
            max_count = count;
            favorite_id = inventory[i];
        }
    }
    
    if (max_count <= 1) return -1; // Все уникальны или пусто
    return favorite_id;
}

// Формирование текста дневника (чистая логика, без вывода)
void format_diary_entry(char *buffer, int buffer_size, const char *farmer_name, 
                        int day, int hour, const int inventory[], 
                        char item_names[][MAX_NAME_LEN], int inv_size) {
    int offset = 0;
    offset += snprintf(buffer + offset, buffer_size - offset, 
                       "=== Запись в дневнике ===\nФермер: %s\nДень: %d, Час: %02d:00\nИнвентарь:\n", 
                       farmer_name, day, hour);
    
    int empty = 1;
    for (int i = 0; i < inv_size; i++) {
        if (inventory[i] != EMPTY_SLOT_ID) {
            offset += snprintf(buffer + offset, buffer_size - offset, 
                               "  - Слот %d: %s\n", i, item_names[inventory[i]]);
            empty = 0;
        }
    }
    if (empty) {
        snprintf(buffer + offset, buffer_size - offset, "  - (пусто)\n");
    }
    snprintf(buffer + offset, buffer_size - offset, "========================\n\n");
}

// Обработка шаблона NPC (Вариант 4) - чистая логика
void process_npc_template(const char *input_text, char *output_text, int out_size, 
                          const char *farmer_name, int day, 
                          const int inventory[], char item_names[][MAX_NAME_LEN]) {
    const char *p = input_text;
    char *res_ptr = output_text;
    int remaining = out_size - 1;

    while (*p && remaining > 0) {
        if (strncmp(p, "<NAME>", 6) == 0) {
            int len = strlen(farmer_name);
            if (len > remaining) len = remaining;
            strncpy(res_ptr, farmer_name, len);
            res_ptr += len; remaining -= len; p += 6;
        } else if (strncmp(p, "<DAY>", 5) == 0) {
            char day_str[12];
            int len = snprintf(day_str, sizeof(day_str), "%d", day);
            if (len > remaining) len = remaining;
            strncpy(res_ptr, day_str, len);
            res_ptr += len; remaining -= len; p += 5;
        } else if (strncmp(p, "<ITEM>", 6) == 0) {
            const char *item = item_names[inventory[0]];
            int len = strlen(item);
            if (len > remaining) len = remaining;
            strncpy(res_ptr, item, len);
            res_ptr += len; remaining -= len; p += 6;
        } else {
            *res_ptr++ = *p++;
            remaining--;
        }
    }
    *res_ptr = '\0';
}

// --- Реализация вывода (Интерфейс) ---

void print_clock(int day, int hour) {
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

void print_inventory(const int inventory[], char item_names[][MAX_NAME_LEN], int size) {
    printf("\n--- Инвентарь ---\n");
    for (int i = 0; i < size; i++) {
        printf("Слот %d: [%d] - %s\n", i, inventory[i], item_names[inventory[i]]);
    }
}

void print_search_result(const char *search_name, int item_id, 
                         const int inventory[], int size, 
                         char item_names[][MAX_NAME_LEN]) {
    if (item_id == -1) {
        printf("Предмет с названием '%s' не найден в каталоге.\n", search_name);
        return;
    }

    printf("Предмет '%s' (ID %d) найден в слотах: ", search_name, item_id);
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (inventory[i] == item_id) {
            printf("%d ", i);
            found = 1;
        }
    }
    if (!found) printf("нет (отсутствует в рюкзаке)");
    printf("\n");
}

void print_favorite_resource(int fav_id, const int inventory[], 
                             char item_names[][MAX_NAME_LEN], int size) {
    if (fav_id == -1) {
        printf("Нет любимого ресурса (инвентарь пуст или все предметы уникальны).\n");
        return;
    }
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (inventory[i] == fav_id) count++;
    }
    printf("Любимый ресурс: %s, количество слотов: %d\n", item_names[fav_id], count);
}

// --- Реализация работы с файлами ---

void write_diary_to_file(const char *filename, const char *text) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть %s для записи.\n", filename);
        return;
    }
    fprintf(file, "%s", text);
    fclose(file);
    printf("Состояние записано в %s.\n", filename);
}

void process_variant_4_to_file(const char *in_filename, const char *out_filename, 
                               const char *farmer_name, int day, 
                               const int inventory[], char item_names[][MAX_NAME_LEN]) {
    FILE *in = fopen(in_filename, "r");
    if (in == NULL) {
        printf("Ошибка: не удалось открыть %s.\n", in_filename);
        return;
    }
    FILE *out = fopen(out_filename, "w");
    if (out == NULL) {
        printf("Ошибка: не удалось создать %s.\n", out_filename);
        fclose(in);
        return;
    }

    char line[MAX_BUFFER];
    char result[MAX_BUFFER * 2];
    
    printf("\n--- Результат обработки (Вариант 4) ---\n");
    while (fgets(line, sizeof(line), in)) {
        process_npc_template(line, result, sizeof(result), farmer_name, day, inventory, item_names);
        printf("%s", result);
        fprintf(out, "%s", result);
    }

    fclose(in);
    fclose(out);
    printf("\n--- Результат сохранен в %s ---\n", out_filename);
}

// --- Главная функция ---

int main(void) {
    // Инициализация данных
    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    int inventory[INVENTORY_SIZE];
    char farmer_name[MAX_FARMER_NAME];

    init_inventory(inventory, INVENTORY_SIZE);
    load_item_names(FILENAME_ITEMS, item_names);

    // Ввод имени
    get_string_input("Введите имя фермера: ", farmer_name, sizeof(farmer_name));
    if (strlen(farmer_name) == 0) {
        strncpy(farmer_name, "Безымянный", sizeof(farmer_name));
    }
    printf("Добро пожаловать, %s!\n", farmer_name);

    int current_day = 1;
    int current_hour = 8;
    int choice;

    // Главный цикл
    while (1) {
        printf("\n=== Меню (Фермер: %s) ===\n", farmer_name);
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Любимый ресурс\n");
        printf("[7] Поиск предмета по названию\n");
        printf("[8] Записать состояние в дневник\n");
        printf("[9] Расшифровать старые записи (Вариант 4)\n");

        choice = get_int_input("Ваш выбор: ", 0, 9);

        switch (choice) {
            case 0:
                printf("Выход.\n");
                return 0;

            case 1:
                print_clock(current_day, current_hour);
                break;

            case 2: {
                int hours = get_int_input("Сколько часов потратить? ", 0, 1000);
                current_hour += hours;
                while (current_hour >= 24) {
                    current_hour -= 24;
                    current_day++;
                }
                printf("Прошло %d часов.\n", hours);
                break;
            }

            case 3:
                print_inventory(inventory, item_names, INVENTORY_SIZE);
                break;

            case 4: {
                int index = get_int_input("Введите индекс слота (0-9): ", 0, INVENTORY_SIZE - 1);
                int id = get_int_input("Введите ID предмета (0-9): ", 0, MAX_ITEMS - 1);
                inventory[index] = id;
                printf("Предмет '%s' положен в слот %d.\n", item_names[id], index);
                break;
            }

            case 5: {
                int index = get_int_input("Введите индекс слота для очистки (0-9): ", 0, INVENTORY_SIZE - 1);
                printf("Выброшен предмет: %s\n", item_names[inventory[index]]);
                inventory[index] = EMPTY_SLOT_ID;
                break;
            }

            case 6: {
                int fav_id = calculate_favorite_resource(inventory, INVENTORY_SIZE);
                print_favorite_resource(fav_id, inventory, item_names, INVENTORY_SIZE);
                break;
            }

            case 7: {
                char search_name[MAX_NAME_LEN];
                get_string_input("Введите название предмета: ", search_name, sizeof(search_name));
                int found_id = find_item_id_by_name(search_name, item_names, MAX_ITEMS);
                print_search_result(search_name, found_id, inventory, INVENTORY_SIZE, item_names);
                break;
            }

            case 8: {
                char diary_text[2048];
                format_diary_entry(diary_text, sizeof(diary_text), farmer_name, 
                                   current_day, current_hour, inventory, item_names, INVENTORY_SIZE);
                write_diary_to_file(FILENAME_DIARY, diary_text);
                break;
            }

            case 9:
                process_variant_4_to_file(FILENAME_INPUT, FILENAME_OUTPUT, 
                                          farmer_name, current_day, inventory, item_names);
                break;

            default:
                printf("Неверный пункт меню!\n");
        }
    }

    return 0;
}