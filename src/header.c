/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** header.c
*/

#include "../include/mycorp.h"

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
    if (my_strcmp(".comment", comment_tab[0]) || my_strcmp(".name", name_tab[0]))
        return 84;
    if (name_tab[1][0] != '"' || comment_tab[1][0] != '"')
        return 84;
    if (name_tab[tab_length(name_tab) - 1][my_strlen(name_tab[tab_length(name_tab) - 1]) - 1] != '"')
        return 84;
    if (comment_tab[tab_length(comment_tab) - 1][my_strlen(comment_tab[tab_length(comment_tab) - 1]) - 1] != '"')
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
    char *new_name = malloc(sizeof(char) * (my_strlen(filename) + 3));

    while (i < my_strlen(filename) - 2) {
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
