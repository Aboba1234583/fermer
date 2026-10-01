int find_item_id_by_name(const char *name, char item_names[][MAX_NAME_LEN], int max_items);
void search_item_by_name(char item_names[][MAX_NAME_LEN], const int inventory[], int size);


int find_item_id_by_name(const char *name, char item_names[][MAX_NAME_LEN], int max_items) {
    for (int i = 0; i < max_items; i++) {
        if (strcmp(item_names[i], name) == 0) return i;
    }
    return -1;
}

void search_item_by_name(char item_names[][MAX_NAME_LEN], const int inventory[], int size) {
    char search_name[MAX_NAME_LEN];
    get_string_input("Введите название предмета: ", search_name, sizeof(search_name));

    int found_id = find_item_id_by_name(search_name, item_names, MAX_ITEMS);
    if (found_id == -1) {
        printf("Предмет '%s' не найден в каталоге.\n", search_name);
        return;
    }

    printf("Предмет '%s' (ID %d) найден в слотах: ", search_name, found_id);
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (inventory[i] == found_id) { printf("%d ", i); found = 1; }
    }
    if (!found) printf("нет (отсутствует в рюкзаке)");
    printf("\n");
}


printf("[7] Поиск предмета по названию\n");


choice = get_int_input("Ваш выбор: ", 0, 6);

choice = get_int_input("Ваш выбор: ", 0, 7);


case 7:
    search_item_by_name(item_names, inventory, INVENTORY_SIZE);
    break;