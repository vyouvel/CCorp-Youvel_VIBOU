/*
** EPITECH PROJECT, 2024
** Epitech SYN MyCORP
** File description:
** Structures utilisées
*/

#pragma once

#include "op.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>

typedef enum type_s type_t;
typedef struct parameter_s parameter_t;
typedef struct instruction_s instruction_t;
typedef struct file_s file_t;
typedef struct byte_s byte_t;

typedef enum type_s {
    DIRECT,
    INDIRECT,
    REGISTER
} type_t;

typedef struct byte_s {
    int leftmost;
    int left_middle;
    int right_middle;
    int rightmost;
} byte_t;

typedef struct parameter_s {
    type_t type;
    bool is_index;
    char *label;
    int value;
} parameter_t;

typedef struct instruction_s {
    int size;
    char *label;
    char code;
    parameter_t **parameter;
    bool is_coding_byte;
} instruction_t;

typedef struct file_s {
    int fd;
    int new_fd;
    char *buffer;
    char **content_tab;
    int content_tab_size;
    instruction_t **tab;
} file_t;
