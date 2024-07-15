/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** header.c
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

int is_indirect(char *str)
{
    for (int i = 0; str[i] != 0; i++) {
        if (!(str[i] <= '9' && str[i] >= '0'))
        return 84;
    }
    return 0;
}

int get_value(char *filename, parameter_t *parameter)
{
    if (parameter->type == REGISTER)
        return (atoi(filename + 1));
    if (parameter->type == DIRECT)
        if (filename[1] == ':') {
            parameter->label = my_strdup(filename + 2);
            return -1;
        } else
            return (atoi(filename + 1));
    if (parameter->type == INDIRECT)
        return (atoi(filename));
}

parameter_t **get_parameter(char **tab, instruction_t *instruction)
{
    int i = 0;

    for (i = 1; tab[i] != NULL; i++) {
        if (tab[i][0] == '%')
            instruction->parameter[i -1]->type = DIRECT;
        if (tab[i][0] == 'r')
            instruction->parameter[i - 1]->type = REGISTER;
        if (!is_indirect(tab[i]))
            instruction->parameter[i - 1]->type = INDIRECT;
        instruction->parameter[i - 1]->value = get_value(tab[i], instruction->parameter[i - 1]);
    }
    instruction->parameter[i - 1] = NULL;
    return instruction->parameter;
}

op_t get_op_code(char *op_co)
{
    int i = 0;

    for (i = 0; op_tab[i].code != 0; i++) {
        if (op_co && !my_strcmp(op_co, op_tab[i].mnemonique)) {
            return op_tab[i];
        }
    }
    return op_tab[i];
}
