/*
** EPITECH PROJECT, 2024
** Epitech SYN MyCORP
** File description:
** Fichier d'entête de MyCORP
*/

#pragma once

#include "./op.h"
#include "./struct.h"

typedef enum type_s type_t;
typedef struct parameter_s parameter_t;
typedef struct instruction_s instruction_t;
typedef struct file_s file_t;
typedef struct byte_s byte_t;

bool is_a_alpha_char(char c);
bool is_good_file(file_t *mycorp, char *av);
bool coding_byte_or_not(char code);

void my_putchar(char c);
void print_help(void);
void remplace_comments(char *buffer, int i);
void remove_comments(char *buffer);
void put_word(char *line, int *z, char *buf);
void get_content_in_tab(file_t *mycorp);
void initialise_instruction(char *line, char **tab, instruction_t *instruction);
void check_index_pos(parameter_t *parameter, int j);
void check_index_pos_next(parameter_t *parameter, int j);
void verify_if_index(parameter_t *parameter, char code, int j);
void get_index_pos(file_t *mycorp);
void get_param_size(parameter_t **parameter, int *size);
void compute_size(file_t *mycorp);
void check_label(file_t *mycorp, int i, int j);
void get_labels(file_t *mycorp);
void initialise_struct(file_t *mycorp, char *av);
void copy_name(header_t *header, char *src, int max_size);
void copy_cmt(struct header_s *header, char *src, int max_size);
void write_header(file_t *mycorp, char *filename);

int my_putstr(char const *str);
int tab_length(char **tab);
int count_words(char const *str);
int word_length(char *str, int *z);
int number_of_valid_line(char *buffer);
int size_of_line(char *str);
int fill_line(char *line, int index, char *buffer);
int is_indirect(char *str);
int get_value(char *arg, parameter_t *parameter);
int get_label_number(char *name, file_t *mycorp);
int check_label_val1(file_t *mycorp, int n, int count);
int check_label_val2(file_t *mycorp, int n, int count);
int check_label_val(int n, int count, file_t *mycorp);
int is_redifine_name_comment(file_t *mycorp);
int error_case_name_comment(file_t *mycorp);
int verify_header(file_t *mycorp);
int my_strcmp(const char *y, const char *x);
int my_strncmp(const char *y, const char *x, int n);
int my_strlen(char const *str);
int convert_endian(int num);
int get_prog_size(file_t *mycorp);
int main(int ac, char **av);


char **str_to_word_array(char *str);
char **allocate_tab(char *buffer);
char *recup_name_comment(char *str);
char *my_strdup(char const *src);
char *new_name(char *filename);

parameter_t **get_parameter(char **tab, instruction_t *instruction);
op_t get_op_code(char *op_co);

