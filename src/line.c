/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** line.c
*/

#include "../include/mycorp.h"

int number_of_valid_line(char *buffer)
{
    int i = 0;
    int lines = 0;

    for (i = 0; buffer[i] == '\n'; i++);
    for (i; buffer[i] != 0; i++) {
        if (buffer[i] == '\n' && buffer[i - 1] != '\n')
            lines++;
    }
    if (buffer[my_strlen(buffer) - 1] != '\n')
        lines++;
    return lines;
}

int size_of_line(char *str)
{
    int size = 0;

    for (int i = 0; str[i] != '\0' && str[i] != '\n'; i++) {
        size++;
    }
    return size;
}

char **allocate_tab(char *buffer)
{
    int lines = number_of_valid_line(buffer);
    char **tab = (char **)malloc(sizeof(char *) * (lines + 1));

    return tab;
}

int fill_line(char *line, int index, char *buffer)
{
    int i = 0;

    while (buffer[index] != '\0') {
        if (buffer[index] != '\n') {
            line[i] = buffer[index];
            index++;
            i++;
            continue;
        } else {
            line[i] = '\0';
            index++;
            return index;
        }
    }
    line[i] = '\0';
    return index;
}

void get_content_in_tab(file_t *mycorp)
{
    int i;
    int j = 0;
    int index = 0;

    mycorp->content_tab = allocate_tab(mycorp->buffer);
    for (i = 0; i < number_of_valid_line(mycorp->buffer); i++) {
        if (!size_of_line(mycorp->buffer + index)) {
            i--;
            index++;
            continue;
        }
        mycorp->content_tab[i] = (char *)malloc(size_of_line(mycorp->buffer + index) + 1);
        index = fill_line(mycorp->content_tab[i], index, mycorp->buffer);
    }
    mycorp->content_tab[i] = NULL;
    mycorp->content_tab_size = i;
    return;
}
