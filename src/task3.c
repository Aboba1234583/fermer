#define FILENAME_INPUT "input.txt"
#define FILENAME_OUTPUT "output.txt"
#define MAX_BUFFER 512


void process_npc_template(const char *input_text, char *output_text, int out_size,
                          const char *farmer_name, int day,
                          const int inventory[], char item_names[][MAX_NAME_LEN]);
void process_variant_4_to_file(const char *in_filename, const char *out_filename,
                               const char *farmer_name, int day,
                               const int inventory[], char item_names[][MAX_NAME_LEN]);


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


printf("[9] Расшифровать старые записи (Вариант 4)\n");


choice = get_int_input("Ваш выбор: ", 0, 8);
choice = get_int_input("Ваш выбор: ", 0, 9);


    process_variant_4_to_file(FILENAME_INPUT, FILENAME_OUTPUT,
                              farmer_name, current_day, inventory, item_names);
    break;