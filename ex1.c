int current_hour = 8;
    int inventory[INVENTORY_SIZE] = {4, 6, 0, 2, 0, 3, 8, 0, 1, 0};

    const char *item_names[10] = {
        "пусто", "дерево", "камень", "семена",
        "лопата", "грабли", "тяпка", "телега",
        "лейка", "корзина"
    };

    int choice;
    

    while (1) {
@@ -54,9 +60,36 @@ int main(void) {
                break;
            }
            case 3:
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    printf("Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]]);
                }
                break;
            case 4:
            case 4: {
                int index, id;
                printf("Введите индекс слота (0-%d): ", INVENTORY_SIZE - 1);
                if (scanf("%d", &index) != 1) {
                    printf("Ошибка: нужно ввести число!\n");
                    while (getchar() != '\n');
                    break;
                }
                if (index < 0 || index >= INVENTORY_SIZE) {
                    printf("Ошибка: индекс вне границ массива (0-%d)!\n", INVENTORY_SIZE - 1);
                    break;
                }
                printf("Введите ID предмета (0-9): ");
                if (scanf("%d", &id) != 1) {
                    printf("Ошибка: нужно ввести число!\n");
                    while (getchar() != '\n');
                    break;
                }
                if (id < 0 || id > 9) {
                    printf("Ошибка: такого ID не существует!\n");
                    break;
                }
                inventory[index] = id;
                printf("Предмет %d положен в слот %d.\n", id, index);
                break;
            }
            case 5:
                break;
            case 6: