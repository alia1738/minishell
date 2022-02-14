NAME = minishell
SRC = minishell.c get_cmd_path.c simple_helpers.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
LINK_LIBFT = -Llibft -lft
CFLAGS = -Wall -Wextra -Werror -MD

all: $(NAME)
-include $(DEP)
$(NAME): $(OBJ)
	$(MAKE) -sC libft all
	gcc $(OBJ) $(LINK_LIBFT) -lreadline -o $(NAME)
clean:
	$(MAKE) -sC libft fclean
	rm -f $(wildcard *o) $(wildcard *d)
fclean: clean
	$(MAKE) -sC libft fclean
	rm -f $(NAME)
re: fclean all