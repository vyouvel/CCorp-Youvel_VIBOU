/*
** EPITECH PROJECT, 2022
** my_my_strdup.c
** File description:
** my_my_strdup.c
*/

#include "mycorp.h"

char *my_strdup(char const *src)
{
    char *cpy;
    int j;
    int i = my_strlen(src);
    cpy = malloc(sizeof(char) * (i + 1));

    for (j = 0; src[j] != '\0'; ) {
        cpy[j] = src[j];
        j++;
    }
    cpy[i] = '\0';

    return (cpy);
    free(cpy);
}
