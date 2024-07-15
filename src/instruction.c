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
    int pow = strlen(bin) - 1;

    for (int i = 0; bin[i] != '\0'; i++) {
        result += (bin[i] - 48) * (my_power(2, pow));
        pow--;
    }
    return result;
}


short short_swap(short value)
{
    short left = value & 0xFF;
    short right = (value >> 8) & 0xFF;
    short result;

    result = (left << 8) | right;
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

void nextt(char *ptr)
{
    if (ptr != NULL)
        free(ptr);
}

void free_file(file_t *mycorp)
{
    free(mycorp->buffer);
    for (int i = 0; mycorp->tab[i] != NULL; i++) {
        for (int j = 0; mycorp->tab[i]->parameter[j] != NULL; j++) {
            nextt(mycorp->tab[i]->parameter[j]->label);
            free(mycorp->tab[i]->parameter[j]);
        }
        free(mycorp->tab[i]->parameter);
        nextt(mycorp->tab[i]->label);
        free(mycorp->tab[i]);
    }
    for (int i = 0; mycorp->content_tab[i] != NULL; i++) {
        free(mycorp->content_tab[i]);
    }
    free(mycorp->content_tab);
    free(mycorp->tab);
    close(mycorp->fd);
    close(mycorp->new_fd);
}

int main(int ac, char **av)
{
    file_t mycorp;

    if (ac == 2 && av[1][0] == '-' && av[1][1] == 'h') {
        print_help();
        return 0;
    }
    if (ac < 2) {
        write(2, "Not enough parameters, check the usage with \'-h\'\n", 49);
        return 84;
    }
    if (is_good_file(&mycorp, av[1]) == false) {
        write(2, "Not a recognized file format\n", 29);
        close(mycorp.fd);
        if (mycorp.buffer != NULL)
            free(mycorp.buffer);
        return 84;
    }
    initialise_struct(&mycorp, av[1]);
    if (verify_header(&mycorp) == 84)
        return 84;
    write_header(&mycorp, av[1]);
    if (verify_label(&mycorp) == 84)
        return 84;
    if (verify_every_param(&mycorp) == 84)
        return 84;
    write_instructions(&mycorp);
    free_file(&mycorp);
}
