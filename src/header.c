/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** header.c
*/

#include "../include/mycorp.h"

void initialise_instruction(char *line, char **tab, instruction_t *instruction)
{
    op_t op;

    if (tab[0][strlen(tab[0]) - 1] == ':') {
        instruction->label = strdup(tab[0]);
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
        if (mycorp->tab[i]->label && !strncmp(mycorp->tab[i]->label, name, strlen(name)))
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
    mycorp->tab = (instruction_t**)malloc(sizeof(instruction_t *) *
    (j - 1));
    for (i = 0; i < j - 2; i++) {
        mycorp->tab[i] = (instruction_t *)malloc(sizeof(instruction_t));
    }
    mycorp->tab[i] = NULL;
    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        tab = str_to_word_array(mycorp->content_tab[i + 2]);
        initialise_instruction(mycorp->content_tab[i + 2], tab, mycorp->tab[i]);
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
        if (!strcmp(line_tab[0], ".comment"))
            return 84;
        if (!strcmp(line_tab[0], ".name"))
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

    if (!strlen(name) || !strlen(cmt))
        return 84;
    if (strlen(name) > PROG_NAME_LENGTH || strlen(cmt) > COMMENT_LENGTH)
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
    if (strcmp(".comment", comment_tab[0]) || strcmp(".name", name_tab[0]))
        return 84;
    if (name_tab[1][0] != '"' || comment_tab[1][0] != '"')
        return 84;
    if (name_tab[tab_length(name_tab) - 1][strlen(name_tab[tab_length(name_tab) - 1]) - 1] != '"')
        return 84;
    if (comment_tab[tab_length(comment_tab) - 1][strlen(comment_tab[tab_length(comment_tab) - 1]) - 1] != '"')
        return 84;
    if (error_case_name_comment(mycorp) == 84)
        return 84;
    return 0;
}

char *new_name(char *filename)
{
    char cor[] = ".cor";
    int i = 0;
    int j = 0;
    char *new_name = malloc(sizeof(char) * (strlen(filename) + 3));

    while (i < strlen(filename) - 2) {
        new_name[i] = filename[i];
        i++;
    }
    while (cor[j] != '\0') {
        new_name[i] = cor[j];
        i++;
        j++;
    }
    new_name[i] = '\0';
    return new_name;
}

int get_prog_size(file_t *mycorp)
{
    int size = 0;

    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        size += mycorp->tab[i]->size;
    }
    return size;
}

void copy_name(header_t *header, char *src, int max_size)
{
    int i = 0;

    while (src[i] != '\0') {
        header->prog_name[i] = src[i];
        i++;
    }
    while (i < max_size) {
        header->prog_name[i] = '\0';
        i++;
    }
}

void copy_cmt(struct header_s *header, char *src, int max_size)
{
    int i = 0;

    while (src[i] != '\0') {
        header->comment[i] = src[i];
        i++;
    }
    while (i < max_size) {
        header->comment[i] = '\0';
        i++;
    }
}

void write_header(file_t *mycorp, char *filename)
{
    header_t *header = malloc(sizeof(header_t));
    char *new = new_name(filename);
    char *name = recup_name_comment(mycorp->content_tab[0]);
    char *comment = recup_name_comment(mycorp->content_tab[1]);
    int size = get_prog_size(mycorp);
    int i = 0;

    mycorp->new_fd = open(new, O_CREAT | O_RDONLY | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
    header->magic = convert_endian(CORP_EXEC_MAGIC);
    while (name[i] != '\0') {
        header->prog_name[i] = name[i];
        i++;
    }
    while (i < PROG_NAME_LENGTH) {
        header->prog_name[i] = '\0';
        i++;
    }
    i = 0;
    while (comment[i] != '\0') {
        header->comment[i] = comment[i];
        i++;
    }
    while (i < COMMENT_LENGTH) {
        header->comment[i] = '\0';
        i++;
    }
    header->prog_size = convert_endian(size);
    write(mycorp->new_fd, header, sizeof(struct header_s));
}
