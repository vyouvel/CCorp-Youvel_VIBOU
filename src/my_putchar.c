/*
** EPITECH PROJECT, 2024
** CCorp-Youvel_VIBOU
** File description:
** my_putchar.c
*/

#include "../include/mycorp.h"

void my_putchar(char c)
{
    write(1, &c, 1);
}
