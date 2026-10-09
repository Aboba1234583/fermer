#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVENTORY_SIZE 10         
#define MAX_ITEMS 10              
#define MAX_NAME_LEN 32           
#define MAX_FARMER_NAME 32        
#define MAX_BUFFER 512            
#define MAX_RECIPE_INGREDIENTS 8  

#define FILENAME_ITEMS  "items.txt"   
#define FILENAME_DIARY  "diary.txt"   
#define FILENAME_INPUT  "input.txt"   
#define FILENAME_OUTPUT "output.txt"  

#define EMPTY_SLOT_ID 0


//утилиты ввода(анонс)
void clear_input_buffer(void);                
int  get_int_input(const char *prompt, int min, int max); 
void get_string_input(const char *prompt, char *buffer, int max_len); 

//Логика 
void load_item_names(const char *filename, char item_names[][MAX_NAME_LEN]);
void init_inventory(int inventory[], int size); 
int  find_item_id_by_name(const char *name, char item_names[][MAX_NAME_LEN], int max_items); 
int  calculate_favorite_resource(const int inventory[], int size); 

//очистка от мусора по id (объявление)
int remove_items_by_id(int inventory[], int size, int item_id);

//проверка рецептов крафта
int  count_in_inventory(const int inventory[], int size, int item_id);
void process_recipes(const char *in_filename, const char *out_filename,
                     const int inventory[], int inv_size,
                     char item_names[][MAX_NAME_LEN]);

//Вывод
void print_clock(int day, int hour);
void print_inventory(const int inventory[], char item_names[][MAX_NAME_LEN], int size);
void print_search_result(const char *search_name, int item_id,
                         const int inventory[], int size,
                         char item_names[][MAX_NAME_LEN]);
void print_favorite_resource(int fav_id, const int inventory[],
                             char item_names[][MAX_NAME_LEN], int size);

                             
// работа с файлами

void write_diary_to_file(const char *filename, const char *text);

// утилиты ввода(тело)
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
            printf("Ошибка: неверный формат. Введите число.\n");
            clear_input_buffer();                  
        }
    }
}

void get_string_input(const char *prompt, char *buffer, int max_len) {
    printf("%s", prompt); 
    if (fgets(buffer, max_len, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; 
    } else {
        buffer[0] = '\0';                    
    }
}

void init_inventory(int inventory[], int size) {
    int default_inv[INVENTORY_SIZE] = {4, 6, 0, 2, 0, 3, 8, 0, 1, 0};
    for (int i = 0; i < size; i++) {
        inventory[i] = default_inv[i];
    }
}

void load_item_names(const char *filename, char item_names[][MAX_NAME_LEN]) {
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
        printf("[Предупреждение] %s не найден. Используются дефолтные названия.\n", filename);
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
    return -1;
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

    if (max_count <= 1) return -1;  
    return favorite_id;
}

//очистка от мусора (5 варик 2 лаба)

int remove_items_by_id(int inventory[], int size, int item_id) {
    int removed_count = 0;                       
    for (int i = 0; i < size; i++) {             
        if (inventory[i] == item_id) {
            inventory[i] = EMPTY_SLOT_ID;        
            removed_count++;         
        }
    }
    return removed_count;        
}

// проверка рецептов крафта(6 варик 3 лаба)

int count_in_inventory(const int inventory[], int size, int item_id) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (inventory[i] == item_id) count++;
    }
    return count;
}

void process_recipes(const char *in_filename, const char *out_filename,
                     const int inventory[], int inv_size,
                     char item_names[][MAX_NAME_LEN]) {

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
    printf("\n--- Проверка рецептов крафта ---\n");

    while (fgets(line, sizeof(line), in)) {
        char *colon = strchr(line, ':');
        if (colon == NULL) continue;

        *colon = '\0';
        char *recipe_name = line;
        while (*recipe_name == ' ' || *recipe_name == '\t') recipe_name++;
        char *end = recipe_name + strlen(recipe_name) - 1;
        while (end > recipe_name && (*end == ' ' || *end == '\t')) *end-- = '\0';

        int required_ids[MAX_RECIPE_INGREDIENTS];
        int required_count[MAX_RECIPE_INGREDIENTS];
        int unique_count = 0;

        char *token = strtok(colon + 1, " \t\r\n");
        while (token != NULL && unique_count < MAX_RECIPE_INGREDIENTS) {
            int id = atoi(token);                     
            if (id > 0 && id < MAX_ITEMS) {           
                int found = -1;
                for (int k = 0; k < unique_count; k++) {
                    if (required_ids[k] == id) { found = k; break; }
                }
                if (found >= 0) {
                    required_count[found]++;          
                } else {
                    required_ids[unique_count] = id;  
                    required_count[unique_count] = 1;
                    unique_count++;
                }
            }
            token = strtok(NULL, " \t\r\n");         
        }

        int can_craft = 1;                   
        char missing[MAX_BUFFER] = "";      

        for (int k = 0; k < unique_count; k++) {
            int have = count_in_inventory(inventory, inv_size, required_ids[k]);
            int need = required_count[k];

            if (have < need) {             
                can_craft = 0;
                int lack = need - have;       
                char tmp[128];
                snprintf(tmp, sizeof(tmp), "%s x%d ",
                         item_names[required_ids[k]], lack);
                strncat(missing, tmp, sizeof(missing) - strlen(missing) - 1);
            }
        }

        char result[MAX_BUFFER * 2];
        if (can_craft) {
            snprintf(result, sizeof(result), "[ДОСТУПНО] %s\n", recipe_name);
        } else {
            snprintf(result, sizeof(result),
                     "[НЕ ХВАТАЕТ РЕСУРСОВ] %s (не хватает: %s)\n",
                     recipe_name, missing);
        }

        printf("%s", result);
        fprintf(out, "%s", result);
    }

    fclose(in);
    fclose(out);
    printf("--- Результат сохранён в %s ---\n", out_filename);
}

// функции ввода

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
        printf("Предмет '%s' не найден в каталоге.\n", search_name);
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
        printf("Нет любимого ресурса (пусто или все уникальны).\n");
        return;
    }
    int count = 0;
    for (int i = 0; i < size; i++)
        if (inventory[i] == fav_id) count++;
    printf("Любимый ресурс: %s, слотов: %d\n", item_names[fav_id], count);
}

// файлы инпут и итем

void write_diary_to_file(const char *filename, const char *text) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть %s.\n", filename);
        return;
    }
    fprintf(file, "%s", text);
    fclose(file);
    printf("Записано в %s.\n", filename);
}

// функции
int main(void) {
    char item_names[MAX_ITEMS][MAX_NAME_LEN];   
    int  inventory[INVENTORY_SIZE];            
    char farmer_name[MAX_FARMER_NAME];

    init_inventory(inventory, INVENTORY_SIZE);       
    load_item_names(FILENAME_ITEMS, item_names);     

    //имя
    get_string_input("Введите имя фермера: ", farmer_name, sizeof(farmer_name));
    if (strlen(farmer_name) == 0) {
        strncpy(farmer_name, "Безымянный", sizeof(farmer_name));
    }
    printf("Добро пожаловать, %s!\n", farmer_name);

    int current_day = 1;
    int current_hour = 8;
    int choice;

    // меню
    while (1) {
        printf("\n=== Меню (Фермер: %s) ===\n", farmer_name);
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет\n");
        printf("[5] Очистка от мусора (по ID)\n");
        printf("[6] Любимый ресурс\n");
        printf("[7] Поиск предмета по названию\n");
        printf("[8] Записать состояние в дневник\n");
        printf("[9] Проверка рецептов крафта\n");

        choice = get_int_input("Ваш выбор: ", 0, 9);

        switch (choice) {
            case 0:   // выход
                printf("Выход.\n");
                return 0;

            case 1:   // часы
                print_clock(current_day, current_hour);
                break;

            case 2: { // промотка времени
                int hours = get_int_input("Сколько часов потратить? ", 0, 1000);
                current_hour += hours;
                // переводим лишние часы в дни
                while (current_hour >= 24) {
                    current_hour -= 24;
                    current_day++;
                }
                printf("Прошло %d часов.\n", hours);
                break;
            }

            case 3:   // инвентарь
                print_inventory(inventory, item_names, INVENTORY_SIZE);
                break;

            case 4: { // положить предмет
                int index = get_int_input("Индекс слота (0-9): ", 0, INVENTORY_SIZE - 1);
                int id    = get_int_input("ID предмета (0-9): ", 0, MAX_ITEMS - 1);
                inventory[index] = id;
                printf("'%s' положен в слот %d.\n", item_names[id], index);
                break;
            }

            // очистяка от мусора(вызов)
            case 5: {
                int id = get_int_input("Введите ID предмета для очистки (0-9): ", 
                                       0, MAX_ITEMS - 1);

                int removed = remove_items_by_id(inventory, INVENTORY_SIZE, id);

                if (removed > 0) {
                    printf("Очищено слотов с предметом '%s' (ID %d): %d\n",
                           item_names[id], id, removed);
                } else {
                    printf("Предмет '%s' (ID %d) не найден в рюкзаке. Очищено: 0\n",
                           item_names[id], id);
                }
                break;
            }

            case 6: { // любимый ресурс
                int fav_id = calculate_favorite_resource(inventory, INVENTORY_SIZE);
                print_favorite_resource(fav_id, inventory, item_names, INVENTORY_SIZE);
                break;
            }

            case 7: { // поиск по названию
                char search_name[MAX_NAME_LEN];
                get_string_input("Введите название предмета: ", search_name, sizeof(search_name));
                int found_id = find_item_id_by_name(search_name, item_names, MAX_ITEMS);
                print_search_result(search_name, found_id, inventory, INVENTORY_SIZE, item_names);
                break;
            }

            case 8: { // запись дневника
                char diary[2048];
                int offset = 0;
                // Пишем последовательно в буфер, offset — текущая позиция
                offset += snprintf(diary + offset, sizeof(diary) - offset,
                                   "=== Дневник ===\nФермер: %s\nДень: %d, Час: %02d:00\nИнвентарь:\n",
                                   farmer_name, current_day, current_hour);
                int empty = 1;
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i] != EMPTY_SLOT_ID) {
                        offset += snprintf(diary + offset, sizeof(diary) - offset,
                                           "  - Слот %d: %s\n", i, item_names[inventory[i]]);
                        empty = 0;
                    }
                }
                if (empty) {
                    snprintf(diary + offset, sizeof(diary) - offset, "  - (пусто)\n");
                }
                write_diary_to_file(FILENAME_DIARY, diary);
                break;
            }
            //обработка рецептов крафта
            case 9:
                process_recipes(FILENAME_INPUT, FILENAME_OUTPUT,
                                inventory, INVENTORY_SIZE, item_names);
                break;

            default:
                printf("Неверный пункт меню!\n");
        }
    }

    return 0;
}