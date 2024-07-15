/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** word.c
*/

#include "../include/mycorp.h"

int count_words(char const *str)
{
    int i = 0;
    int n = 0;

    for (; str[i] != '\0'; i++) {
        if (is_a_alpha_char(str[i]) == false && i != 0 &&
        is_a_alpha_char(str[i - 1]) == true) {
            n++;
        }
    }
    if (is_a_alpha_char(str[i - 1]) == true) {
        return (n + 1);
    } else {
        return n;
    }
}

int word_length(char *str, int *z)
{
    int i;
    int n = 0;

    while (is_a_alpha_char(str[*z]) == false)
        *z = *z + 1;
    i = *z;
    while (str[i] != '\0' && is_a_alpha_char(str[i]) == true) {
        n++;
        i++;
    }
    return n;
}

void put_word(char *line, int *z, char *buf)
{
    int i = 0;

    while (buf[*z] != '\0') {
        if (is_a_alpha_char(buf[*z]) == true) {
            line[i] = buf[*z];
            (*z)++;
            i++;
            continue;
        }
        if (is_a_alpha_char(buf[*z]) == false) {
            line[i] = '\0';
            (*z)++;
            return;
        }
    }
    line[i] = '\0';
}

char **str_to_word_array(char *str)
{
    int i;
    int j = 0;
    int z = 0;
    int words = count_words(str);
    char **tab = (char **)malloc(sizeof(char *) * (words + 1));

    for (i = 0; i < words; i++) {
        tab[i] = (char *)malloc(word_length(str, &z) + 1);
        put_word(tab[i], &z, str);
    }
    tab[i] = NULL;
    return tab;
}

bool is_good_file(file_t *mycorp, char *av)
{
    struct stat info;
    int size;
    int n = 0;

    mycorp->fd = open(av, O_RDONLY);
    if (mycorp->fd < 0)
        return false;
    n = stat(av, &info);
    size = info.st_size;
    mycorp->buffer = malloc(sizeof(char) * (size + 1));
    read(mycorp->fd, mycorp->buffer, size);
    mycorp->buffer[size] = '\0';
    remove_comments(mycorp->buffer);
    if (my_strlen(mycorp->buffer) == 0)
        return false;
    return true;
}
