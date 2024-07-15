/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** parsing.c
*/

#include "../include/mycorp.h"

void print_help(void)
{
    write(1, "USAGE\n", 6);
    return;
}

int tab_length(char **tab)
{
    int size = 0;

    while (tab[size])
        size++;
    return size;
}

void remplace_comments(char *buffer, int i)
{
    for (; buffer[i] != 0 && buffer[i] != '\n'; i++) {
        buffer[i] = '\n';
    }
}

void remove_comments(char *buffer)
{
    for (int i = 0; buffer[i]; i++) {
        if (buffer[i] == '#')
            remplace_comments(buffer, i);
    }
}

bool is_a_alpha_char(char c)
{
    if (c != ' ' && c != '\t' && c != ',')
        return true;
    else {
        return false;
    }
}

int convert_endian(int num)
{
    byte_t bytes;
    int result = 0;

    bytes.leftmost = (num & 0x000000FF);
    bytes.left_middle = (num & 0x0000FF00) >> 8;
    bytes.right_middle = (num & 0x00FF0000) >> 16;
    bytes.rightmost = (num & 0xFF000000) >> 24;
    bytes.leftmost <<= 24;
    bytes.left_middle <<= 16;
    bytes.right_middle <<= 8;
    bytes.rightmost <<= 0;
    result = (bytes.leftmost | bytes.left_middle | bytes.right_middle
    | bytes.rightmost);
    return result;
}

int count_words(char const *str)
{
    int i = 0;
    int n = 0;

    for (; str[i] != '\0'; i++) {
        if (is_a_alpha_char(str[i]) == false && i != 0 && is_a_alpha_char(str[i - 1]) == true) {
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
