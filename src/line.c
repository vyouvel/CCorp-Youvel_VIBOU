/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** header.c
*/

#include "../include/mycorp.h"

bool coding_byte_or_not(char code)
{
    if (code == 1 || code == 0x09 || code == 0x0c || code == 0x0f)
        return false;
    else
        return true;
}

void initialise_instruction(char *line, char **tab, instruction_t *instruction)
{
    op_t op;

    if (tab[0][my_strlen(tab[0]) - 1] == ':') {
        instruction->label = my_strdup(tab[0]);
        tab = tab + 1;
    } else {
        instruction->label = NULL;
    }
    instruction->parameter = malloc(sizeof(parameter_t *) * tab_length(tab));
    for (int i = 0; i < tab_length(tab) - 1; i++) {
        instruction->parameter[i] = malloc(sizeof(parameter_t));
        instruction->parameter[i]->label = NULL;
    }
    op = get_op_code(tab[0]);
    instruction->code = op.code;
    instruction->is_coding_byte = coding_byte_or_not(op.code);
    instruction->parameter = get_parameter(tab, instruction);
}

void check_index_pos(parameter_t *parameter, int j)
{
    if (j == 2)
        parameter->is_index = false;
    if (j == 0 || j == 1)
        if (parameter->type != REGISTER)
            parameter->is_index = true;
}

void check_index_pos_next(parameter_t *parameter, int j)
{
    if (j == 0)
        parameter->is_index = false;
    if (j == 1 || j == 2)
        if (parameter->type != REGISTER)
            parameter->is_index = true;
}

void verify_if_index(parameter_t *parameter, char code, int j)
{
    if (code == 0x0c || code == 0x09 || code == 0x0f)
        parameter->is_index = true;
    if (code == 0x0a || code == 0x0e)
        check_index_pos(parameter, j);
    if (code == 0x0b)
        check_index_pos_next(parameter, j);
}

void get_index_pos(file_t *mycorp)
{
    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        for (int j = 0; mycorp->tab[i]->parameter[j] != NULL; j++) {
            mycorp->tab[i]->parameter[j]->is_index = false;
            verify_if_index(mycorp->tab[i]->parameter[j], mycorp->tab[i]->code, j);
        }
    }
}

void get_param_size(parameter_t **parameter, int *size)
{
    for (int i = 0; parameter[i] != NULL; i++) {
        if (parameter[i]->type == INDIRECT) {
            *size += IND_SIZE;
            continue;
        }
        if (parameter[i]->type == DIRECT && parameter[i]->is_index) {
            *size += IND_SIZE;
            continue;
        }
        if (parameter[i]->type == DIRECT && !(parameter[i]->is_index)) {
            *size += DIR_SIZE;
            continue;
        }
        if (parameter[i]->type == REGISTER) {
            (*size)++;
            continue;
        }
    }
}

void compute_size(file_t *mycorp)
{
    int size = 1;

    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        if (mycorp->tab[i]->parameter[0] == NULL) {
            mycorp->tab[i]->size = 0;
            continue;
        }
        size = 1;
        if (mycorp->tab[i]->is_coding_byte)
            size++;
        get_param_size(mycorp->tab[i]->parameter, &size);
        mycorp->tab[i]->size = size;
    }
}

int get_label_number(char *name, file_t *mycorp)
{
    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        if (mycorp->tab[i]->label && !my_strncmp(mycorp->tab[i]->label, name, my_strlen(name)))
            return i;
    }
    return -1;
}

int check_label_val1(file_t *mycorp, int n, int count)
{
    int value = 0;

    for (int i = n + 1; i < count; i++) {
        value += mycorp->tab[i]->size;
    }
    return value;
}

int check_label_val2(file_t *mycorp, int n, int count)
{
    int value = 0;

    for (int i = n - 1; i > count; i--) {
        value += mycorp->tab[i]->size;
    }
    return value;
}

int check_label_val(int n, int count, file_t *mycorp)
{
    int value = 0;

    if (count == -1)
        return -1;
    if (n < count) {
        value += mycorp->tab[n]->size;
        value += check_label_val1(mycorp, n, count);
    } else if (n > count) {
        value += mycorp->tab[count]->size;
        value += check_label_val2(mycorp, n, count);
        value *= -1;
    }
    return value;
}
