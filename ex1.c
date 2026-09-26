if (favorite_id == 0) {
    
                    printf("Инвентарь пуст — любимого ресурса нет.\n");
                } else if (max_count == 1) {
                    printf("Нет любимого ресурса — все предметы уникальны.\n");
                } else {
                    printf("Любимый ресурс: ID %d (%s), количество слотов: %d\n",
                           favorite_id, item_names[favorite_id], max_count);
                        favorite_id, item_names[favorite_id], max_count);
                }
                break;
            }