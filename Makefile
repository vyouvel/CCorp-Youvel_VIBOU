##
## EPITECH PROJECT, 2024
## CCorp-Youvel_VIBOU
## File description:
## Makefile
##

SRC	=	src/my_putchar.c	\
		src/my_putstr.c	\
		src/my_strdup.c	\
		src/my_strcmp.c	\
		src/my_strlen.c	\
		src/body.c	\
		src/header.c	\
		src/parsing.c	\
		src/main.c	\
		op.c

OBJ	=	$(SRC:.c=.o)

NAME	=	asm

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(OBJ) -g3 -lm

clean:	$(OBJ)
	rm -f $(OBJ) *~

fclean:	clean
	rm -f $(NAME) *~

re:	all clean
