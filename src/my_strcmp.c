/*
** EPITECH PROJECT, 2023
** minishell
** File description:
** minishell
*/

#include "mycorp.h"

int my_strcmp(const char *y, const char *x)
{
    int a;
    for (a = 0; y[a] != '\0' && x[a] != '\0'; a++){
        if (y[a] != x[a]) {
            return (x[a] - y[a]);
        }
        a++;
    }
    return 0;
}

int my_strncmp(const char *y, const char *x, int n)
{
    int a;
    for (a = 0; a < n; a++){
        if (y[a] != x[a]) {
            return (x[a] - y[a]);
        }
        a++;
        return 0;
    }
}
