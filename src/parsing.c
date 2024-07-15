/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** main.c
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

int is_indirect(char *str)
{
    for (int i = 0; str[i] != 0; i++) {
        if (!(str[i] <= '9' && str[i] >= '0'))
        return 84;
    }
    return 0;
}

int get_value(char *arg, parameter_t *parameter)
{
    if (parameter->type == REGISTER)
        return (atoi(arg + 1));
    if (parameter->type == DIRECT)
        if (arg[1] == ':') {
            parameter->label = my_strdup(arg + 2);
            return -1;
        } else
            return (atoi(arg + 1));
    if (parameter->type == INDIRECT)
        return (atoi(arg));
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
        if (!my_strcmp(op_co, op_tab[i].mnemonique)) {
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
            verify_if_index(mycorp->tab[i]->parameter[j],
                mycorp->tab[i]->code, j);
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
        if (mycorp->tab[i]->label && !my_strncmp(mycorp->tab[i]->label, name,
            my_strlen(name)))
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

void check_label(file_t *mycorp, int i, int j)
{
    int n = i;
    int count = 0;

    if (mycorp->tab[i]->parameter[j]->label != NULL) {
        count = get_label_number(mycorp->tab[i]->parameter[j]->label, mycorp);
        mycorp->tab[i]->parameter[j]->value = check_label_val(n, count, mycorp);
    }
}

void get_labels(file_t *mycorp)
{
    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        for (int j = 0; mycorp->tab[i]->parameter[j] != NULL; j++) {
            check_label(mycorp, i, j);
        }
    }
}

void initialise_struct(file_t *mycorp, char *av)
{
    char **tab = NULL;
    int i = 0;
    int j = 0;

    get_content_in_tab(mycorp);
    j = tab_length(mycorp->content_tab);
    mycorp->tab = (instruction_t **)malloc(sizeof(instruction_t *) *
    (j - 1));
    for (i = 0; i < j - 2; i++) {
        mycorp->tab[i] = (instruction_t *)malloc(sizeof(instruction_t));
    }
    mycorp->tab[i] = NULL;
    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        tab = str_to_word_array(mycorp->content_tab[i + 2]);
        initialise_instruction(mycorp->content_tab[i + 2], tab,
            mycorp->tab[i]);
    }
    get_index_pos(mycorp);
    compute_size(mycorp);
    get_labels(mycorp);
}

int is_redifine_name_comment(file_t *mycorp)
{
    char **line_tab = NULL;

    if (tab_length(mycorp->content_tab) < 2)
        return 84;
    for (int i = 2; mycorp->content_tab[i] != NULL; i++) {
        line_tab = str_to_word_array(mycorp->content_tab[i]);
        if (!my_strcmp(line_tab[0], ".comment"))
            return 84;
        if (!my_strcmp(line_tab[0], ".name"))
            return 84;
    }
}

char *recup_name_comment(char *str)
{
    int i = 0;
    int j = 0;
    char *buffer = malloc(sizeof(char) * COMMENT_LENGTH);

    while (str[i] != '"') {
        i++;
    }
    i++;
    while (str[i] != '"') {
        buffer[j] = str[i];
        j++;
        i++;
    }
    buffer[j] = '\0';
    return buffer;
}

int error_case_name_comment(file_t *mycorp)
{
    char *name = recup_name_comment(mycorp->content_tab[0]);
    char *cmt = recup_name_comment(mycorp->content_tab[1]);

    if (!my_strlen(name) || !my_strlen(cmt))
        return 84;
    if (my_strlen(name) > PROG_NAME_LENGTH || my_strlen(cmt) > COMMENT_LENGTH)
        return 84;
    if (is_redifine_name_comment(mycorp) == 84)
        return 84;
}

int verify_header(file_t *mycorp)
{
    char **name_tab = str_to_word_array(mycorp->content_tab[0]);
    char **comment_tab = str_to_word_array(mycorp->content_tab[1]);

    if (tab_length(name_tab) < 2 || tab_length(comment_tab) < 2)
        return 84;
    if (my_strcmp(".comment", comment_tab[0]) ||
        my_strcmp(".name", name_tab[0]))
        return 84;
    if (name_tab[1][0] != '"' || comment_tab[1][0] != '"')
        return 84;
    if (name_tab[tab_length(name_tab) - 1][my_strlen(name_tab
        [tab_length(name_tab) - 1]) - 1] != '"')
        return 84;
    if (comment_tab[tab_length(comment_tab) - 1]
        [my_strlen(comment_tab[tab_length(comment_tab) - 1]) - 1] != '"')
        return 84;
    if (error_case_name_comment(mycorp) == 84)
        return 84;
    return 0;
}
