#define FILENAME_DIARY "diary.txt"


void format_diary_entry(char *buffer, int buffer_size, const char *farmer_name,
                        int day, int hour, const int inventory[],
                        char item_names[][MAX_NAME_LEN], int inv_size);
void write_diary_to_file(const char *filename, const char *text);


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
    if (empty) snprintf(buffer + offset, buffer_size - offset, "  - (пусто)\n");
    snprintf(buffer + offset, buffer_size - offset, "========================\n\n");
}

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


printf("[8] Записать состояние в дневник\n");


choice = get_int_input("Ваш выбор: ", 0, 7);

choice = get_int_input("Ваш выбор: ", 0, 8);


case 8: {
    char diary_text[2048];
    format_diary_entry(diary_text, sizeof(diary_text), farmer_name,
                       current_day, current_hour, inventory, item_names, INVENTORY_SIZE);
    write_diary_to_file(FILENAME_DIARY, diary_text);
    break;
}