/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** main.c
*/

#include "../include/mycorp.h"

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
    if (strlen(mycorp->buffer) == 0)
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
    if (buffer[strlen(buffer) - 1] != '\n')
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
            parameter->label = strdup(filename + 2);
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
        if (op_co && !strcmp(op_co, op_tab[i].mnemonique)) {
            return op_tab[i];
        }
    }
    return op_tab[i];
}

bool coding_byte_or_not(char code)
{
    if (code == 1 || code == 0x09 || code == 0x0c || code == 0x0f)
        return false;
    else
        return true;
}


void nextt(char *ptr)
{
    if (ptr != NULL)
        free(ptr);
}

void free_file(file_t *mycorp)
{
    free(mycorp->buffer);
    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        for (int j = 0; mycorp->tab[i]->parameter[j] != NULL; j++) {
            nextt(mycorp->tab[i]->parameter[j]->label);
            free(mycorp->tab[i]->parameter[j]);
        }
        free(mycorp->tab[i]->parameter);
        nextt(mycorp->tab[i]->label);
        free(mycorp->tab[i]);
    }
    for (int i = 0; mycorp->content_tab[i] != NULL; i++) {
        free(mycorp->content_tab[i]);
    }
    free(mycorp->content_tab);
    free(mycorp->tab);
    close(mycorp->fd);
    close(mycorp->new_fd);
}

int main(int ac, char **av)
{
    file_t mycorp;

    if (ac == 2 && av[1][0] == '-' && av[1][1] == 'h') {
        print_help();
        return 0;
    }
    if (ac < 2) {
        write(2, "Not enough parameters, check the usage with \'-h\'\n", 49);
        return 84;
    }
    if (is_good_file(&mycorp, av[1]) == false) {
        write(2, "Not a recognized file format\n", 29);
        close(mycorp.fd);
        if (mycorp.buffer != NULL)
            free(mycorp.buffer);
        return 84;
    }
    initialise_struct(&mycorp, av[1]);
    if (verify_header(&mycorp) == 84)
        return 84;
    write_header(&mycorp, av[1]);
    if (verify_label(&mycorp) == 84)
        return 84;
    if (verify_every_param(&mycorp) == 84)
        return 84;
    write_instructions(&mycorp);
    free_file(&mycorp);
}
