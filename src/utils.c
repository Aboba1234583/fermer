#include "utils.h"

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
                printf("Ошибка: число от %d до %d.\n", min, max);
            }
        } else {
            printf("Ошибка: введите число.\n");
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

void load_item_names(const char *filename, char item_names[][MAX_NAME_LEN]) {
    const char *default_names[10] = {
        "Пусто", "Дерево", "Камень", "Семена", "Лопата",
        "Грабли", "Тяпка", "Телега", "Лейка", "Корзина"
    };
    for (int i = 0; i < 10; i++) {
        strncpy(item_names[i], default_names[i], MAX_NAME_LEN - 1);
        item_names[i][MAX_NAME_LEN - 1] = '\0';
    }
    FILE *file = fopen(filename, "r");
    if (file == NULL) return;
    char line[MAX_NAME_LEN];
    int id;
    while (fscanf(file, "%d %31s", &id, line) == 2) {
        if (id >= 0 && id < 10) {
            strncpy(item_names[id], line, MAX_NAME_LEN - 1);
            item_names[id][MAX_NAME_LEN - 1] = '\0';
        }
    }
    fclose(file);
}

void print_clock(int day, int hour) {
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

void print_inventory(const int inventory[], char item_names[][MAX_NAME_LEN], int size) {
    printf("\n--- Инвентарь ---\n");
    for (int i = 0; i < size; i++) {
        printf("Слот %d: [%d] - %s\n", i, inventory[i], item_names[inventory[i]]);
    }
}

void write_diary_to_file(const char *filename, const char *text) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) { printf("Ошибка записи.\n"); return; }
    fprintf(file, "%s", text);
    fclose(file);
    printf("Записано в %s.\n", filename);
}