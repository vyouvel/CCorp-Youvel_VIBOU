/*
** EPITECH PROJECT, 2024
** Epitech SYN MyCORP
** File description:
** Main du programme
*/

#include "../include/mycorp.h"

int my_power(int nb, int p)
{
    if (p < 0) {
        return 0;
    }
    if (p == 0) {
        return 1;
    } else {
        return nb * (my_power(nb, p - 1));
    }
}

int decimal_convert(char *bin)
{
    int result = 0;
    int pow = my_strlen(bin) - 1;

    for (int i = 0; bin[i] != '\0'; i++) {
        result += (bin[i] - 48) * (my_power(2, pow));
        pow--;
    }
    return result;
}

op_t get_op(char code)
{
    int i = 0;

    for (i = 0; op_tab[i].code != 0; i++) {
        if (code == op_tab[i].code) {
            return op_tab[i];
        }
    }
    return op_tab[i];
}

short short_swap(short value)
{
    short left = value & 0xFF;
    short right = (value >> 8) & 0xFF;
    short result;

    result = (left << 8) | right;
    return result;
}

int is_elsewhere(char *label, file_t *mycorp, int i)
{
    for (int j = i + 1; mycorp->tab[j] != NULL; j++) {
        if (mycorp->tab[j]->label && !strcmp(mycorp->tab[j]->label, label))
            return 0;
    }
    return 1;
}

int exist(char *label, file_t *mycorp)
{
    if (get_label_number(label, mycorp) == -1)
        return 0;
    return 1;
}

int ckeck_param_label(instruction_t *instruction, file_t *mycorp)
{
    char *label = NULL;

    for (int i = 0; instruction->parameter[i] != NULL; i++) {
        label = instruction->parameter[i]->label;
        if (label != NULL && !exist(label, mycorp))
            return 84;
    }
}

bool find_char(char *str, char c)
{
    for (int i = 0; str[i] != 0; i++) {
        if (str[i] == c)
            return true;
    }
    return false;
}

int char_valid(char *name)
{
    char chars[] = "abcdefghijklmnopqrstuvwxyz_0123456789";

    for (int i = 0; name[i + 1] != 0; i++) {
        if (!find_char(chars, name[i]))
            return 0;
    }
    return 1;
}

int verify_label(file_t *mycorp)
{
    char *label = NULL;

    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        label = mycorp->tab[i]->label;
        if (label != NULL && !char_valid(label))
            return 84;
        if (label != NULL && !is_elsewhere(label, mycorp, i))
            return 84;
        if (ckeck_param_label(mycorp->tab[i], mycorp) == 84)
            return 84;
    }
}

int count_arg(parameter_t **parameter, char code)
{
    int n = 0;
    op_t op = get_op(code);

    for (int i = 0; parameter[i] != NULL; i++) {
        n++;
    }
    if (n != op.nbr_args)
        return 84;
    else
        return 1;
}

int verify_every_param(file_t *mycorp)
{
    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        if (count_arg(mycorp->tab[i]->parameter, mycorp->tab[i]->code) == 84)
            return 84;
    }
    return 1;
}

int build_coding_byte(parameter_t **parameter)
{
    int count = 0;
    char *byte = malloc(sizeof(char) * 9);

    for (int i = 0; parameter[i] != NULL; i++) {
        if (parameter[i]->type == REGISTER)
            strcpy(byte, "01");
        if (parameter[i]->type == DIRECT)
            strcpy(byte, "10");
        if (parameter[i]->type == INDIRECT)
            strcpy(byte, "11");
        byte += 2;
        count += 2;
    }
    byte -= count;
    for (count = count; count < 8; count++)
        byte[count] = '0';
    byte[8] = '\0';
    return decimal_convert(byte);
}

void write_other(parameter_t **parameter, file_t *mycorp)
{
    int value = 0;

    for (int i = 0; parameter[i] != NULL; i++) {
        if (parameter[i]->type == REGISTER) {
            value = (char)parameter[i]->value;
            write(mycorp->new_fd, &value, 1);
        }
        if (parameter[i]->type == DIRECT && parameter[i]->is_index) {
            value = short_swap((short)parameter[i]->value);
            write(mycorp->new_fd, &value, 2);
        }
        if (parameter[i]->type == DIRECT && !(parameter[i]->is_index)) {
            value = convert_endian((int)parameter[i]->value);
            write(mycorp->new_fd, &value, 4);
        }
        if (parameter[i]->type == INDIRECT) {
            value = short_swap((short)parameter[i]->value);
            write(mycorp->new_fd, &value, 2);
        }
    }
}

void write_instructions(file_t *mycorp)
{
    char coding_byte = 0;

    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        if (mycorp->tab[i]->code == 0)
            continue;
        coding_byte = (char)build_coding_byte(mycorp->tab[i]->parameter);
        write(mycorp->new_fd, &(mycorp->tab[i]->code), 1);
        if (mycorp->tab[i]->is_coding_byte)
        write(mycorp->new_fd, &(coding_byte), 1);
        write_other(mycorp->tab[i]->parameter, mycorp);
    }
}
