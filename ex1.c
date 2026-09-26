printf("[6] Любимый ресурс\n");
        printf("Ваш выбор: ");

        scanf("%d", &choice);
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: нужно ввести число!\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 0:
                printf("Выход.\n");
                return 0;
            case 1:
                printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
                break;
            case 2:
            case 2: {
                int hours;
                printf("Сколько часов потратить на работу? ");
                if (scanf("%d", &hours) != 1) {
                    printf("Ошибка: нужно ввести число!\n");
                    while (getchar() != '\n');
                    break;
                }
                if (hours < 0) {
                    printf("Часы не могут быть отрицательными!\n");
                    break;
                }
                current_hour += hours;
                while (current_hour >= 24) {
                    current_hour -= 24;
                    current_day++;
                }
                printf("Прошло %d часов.\n", hours);
                break;
            }
            case 3:
                break;
            case 4: