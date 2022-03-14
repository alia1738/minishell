NAME = minishell
SRC = minishell.c get_cmd_path.c simple_helpers.c command_execution.c execution_utils.c ft_split_custom.c pipex.c builtins.c parser_utils.c expand_dollar_v2.c pipe.c env_export.c local_getenv.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
LINK_LIBFT = -Llibft -lft
LINK_READ = -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include
CFLAGS = -Wall -Wextra -Werror
export LDFLAGS="-L/usr/local/opt/readline/lib" 
export CPPFLAGS="-I/usr/local/opt/readline/include"

all: $(NAME)
-include $(addprefix dep/,$(DEP))

$(NAME): obj dep libft/libft.a $(addprefix obj/,$(OBJ))
	gcc $(addprefix obj/,$(OBJ)) $(LINK_LIBFT) $(LDFLAGS) $(CPPFLAGS) -lreadline -o $(NAME)

libft/libft.a:
	$(MAKE) -sC libft all
	
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