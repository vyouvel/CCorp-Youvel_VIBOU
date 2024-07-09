/*
** EPITECH PROJECT, 2022
** my
** File description:
** my_strlen
*/

#include "../include/mycorp.h"

int my_strlen(char const *str)
{
    int c = 0;
    
    while (str[c] != '\0'){
        c++;
    }
    return c;
}
