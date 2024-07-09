/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** my_putstr.c
*/

#include "../include/mycorp.h"

int my_putstr(char const *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        my_putchar(str[i]);
}
