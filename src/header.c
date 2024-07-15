/*
** EPITECH PROJECT, 2024
** Epitech SYN MyCORP
** File description:
** Main du programme
*/

#include "../include/mycorp.h"

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

    mycorp->new_fd = open(new, O_CREAT | O_RDONLY | O_WRONLY, S_IRWXG | S_IRWXO
                    | S_IRWXU);
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
