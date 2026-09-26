printf("Предмет %d положен в слот %d.\n", id, index);
                break;
            }
            
            case 5:
            case 5: {
                int index;
                printf("Введите индекс слота для очистки (0-%d): ", INVENTORY_SIZE - 1);
                if (scanf("%d", &index) != 1) {
                    printf("Ошибка: нужно ввести число!\n");
                    while (getchar() != '\n');
                    break;
                }
                if (index < 0 || index >= INVENTORY_SIZE) {
                    printf("Ошибка: индекс вне границ массива (0-%d)!\n", INVENTORY_SIZE - 1);
                    break;
                }
                printf("Выброшен предмет: %d (%s)\n", inventory[index], item_names[inventory[index]]);
                inventory[index] = 0;
                break;
            case 6:
            }
            case 6: {
                int max_count = 0;
                int favorite_id = 0;

                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i] == 0) continue;

                    int count = 0;
                    for (int j = 0; j < INVENTORY_SIZE; j++) {
                        if (inventory[j] == inventory[i]) count++;
                    }

                    if (count > max_count) {
                        max_count = count;
                        favorite_id = inventory[i];
                    }
                }

                if (favorite_id == 0) {
                    printf("Инвентарь пуст — любимого ресурса нет.\n");
                } else {
                    printf("Любимый ресурс: ID %d (%s), количество слотов: %d\n",
                           favorite_id, item_names[favorite_id], max_count);
                }
                break;
            }
            default:
                printf("Неверный пункт меню!\n");
        }