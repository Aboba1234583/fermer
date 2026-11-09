#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 32
#define MAX_BUFFER 512

void clear_input_buffer(void);
int get_int_input(const char *prompt, int min, int max);
void get_string_input(const char *prompt, char *buffer, int max_len);
void load_item_names(const char *filename, char item_names[][MAX_NAME_LEN]);
void print_clock(int day, int hour);
void print_inventory(const int inventory[], char item_names[][MAX_NAME_LEN], int size);
void write_diary_to_file(const char *filename, const char *text);

#endif