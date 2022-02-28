NAME = minishell
SRC = minishell.c get_cmd_path.c simple_helpers.c command_execution.c ft_split_custom.c expand_dollar.c pipex.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
LINK_LIBFT = -Llibft -lft
LINK_READ = -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include
CFLAGS = -Wall -Wextra -Werror
export LDFLAGS="-L/usr/local/opt/readline/lib" 
export CPPFLAGS="-I/usr/local/opt/readline/include"

all: $(NAME)
-include $(addprefix dep/,$(DEP))

$(NAME): directories $(addprefix obj/,$(OBJ))
	$(MAKE) -sC libft all
	gcc $(addprefix obj/,$(OBJ)) $(LINK_LIBFT) $(LDFLAGS) $(CPPFLAGS) -lreadline -o $(NAME)

obj/%.o:%.c
	gcc -c $(CFLAGS) $< -o $@ -MD -MF $(@:obj/%.o=dep/%.d)

clean:
	$(MAKE) -sC libft fclean
	rm -f $(addprefix obj/,$(OBJ)) $(addprefix dep/,$(DEP))

fclean: clean
	$(MAKE) -sC libft fclean
	rm -f $(NAME)

re: fclean all

directories: obj dep
obj:
	mkdir obj
dep:
	mkdir dep