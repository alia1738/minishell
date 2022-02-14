NAME = minishell
SRC = minishell.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
CFLAGS = -Wall -Wextra -Werror -MD

all: $(NAME)
-include $(DEP)
$(NAME): $(OBJ)
	gcc $(OBJ) -lreadline -o $(NAME)
clean:
	rm -f $(wildcard *o) $(wildcard *d)
fclean: clean
	rm -f $(NAME)
re: fclean all