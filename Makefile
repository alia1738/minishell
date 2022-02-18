NAME = minishell
SRC = minishell.c get_cmd_path.c simple_helpers.c split_input.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
LINK_LIBFT = -Llibft -lft
LINK_READ = -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include
CFLAGS = -Wall -Wextra -Werror -MD
export LDFLAGS="-L/usr/local/opt/readline/lib" 
export CPPFLAGS="-I/usr/local/opt/readline/include"

all: $(NAME)
-include $(addprefix dep/,$(DEP))
$(NAME): obj dep $(OBJ)
	$(MAKE) -sC libft all
	gcc $(OBJ) $(LINK_LIBFT) $(LDFLAGS) $(CPPFLAGS) -lreadline -o $(NAME)
obj/%.o:%.c
	gcc -c $(CFLAGS) $< -o $@ $(@:obj/%.o:dep/%.d)
clean:
	$(MAKE) -sC libft fclean
	rm -f $(wildcard *o) $(wildcard *d)
#	rm -f $(addprefix obj/,$(OBJ)) $(addprefix dep/,$(DEP))
fclean: clean
	$(MAKE) -sC libft fclean
	rm -f $(NAME)
re: fclean all
obj:
	mkdir obj
dep:
	mkdir dep