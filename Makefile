NAME = minishell
SRC = minishell.c get_cmd_path.c simple_helpers1.c simple_helpers2.c simple_helpers3.c ft_split_custom.c ft_split_custom_utils.c builtins.c builtins1.c builtins2.c parse_cmds.c parser_utils1.c parser_utils2.c expand_dollar.c pipe_exec.c pipe_exec_utils.c pipe_exec_utils2.c append.c env_export.c env_export_utils.c local_getenv.c free_utils.c memory.c counting_utils.c unset_env.c expand_dollar_utils1.c expand_dollar_utils2.c signals.c sort_cmd_utils.c redirections.c single_cmd_exec_utils.c single_cmd_exec.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
LINK_LIBFT = -Llibft -lft
LINK_READ = -L /usr/local/opt/readline/lib 
CFLAGS = -Wall -Wextra -Werror -I /usr/local/opt/readline/include

all: $(NAME)

-include $(addprefix dep/,$(DEP))

$(NAME): obj dep libft/libft.a $(addprefix obj/,$(OBJ))
	@printf "\033[2;33m\033[3;33m"
	gcc $(addprefix obj/,$(OBJ)) $(LINK_LIBFT) $(LINK_READ) -lreadline -o $(NAME)
	@printf "\e[0m\n"
	@printf "\033[1;35m _______ \033[1;33m _______ \033[1;35m _______ \033[1;33m __   __ \033[1;35m _______ \033[1;33m __   __ \033[1;35m _______ \033[1;33m ___   \033[1;35m   ___     \n"
	@printf "\033[1;35m|  _    |\033[1;33m|   _   |\033[1;35m|  _    |\033[1;33m|  | |  |\033[1;35m|       |\033[1;33m|  | |  |\033[1;35m|       |\033[1;33m|   |    \033[1;35m|   |    \n"
	@printf "\033[1;35m| |_|   |\033[1;33m|  |_|  |\033[1;35m| |_|   |\033[1;33m|  |_|  |\033[1;35m|  _____|\033[1;33m|  |_|  |\033[1;35m|    ___|\033[1;33m|   |    \033[1;35m|   |    \n"
	@printf "\033[1;35m|       |\033[1;33m|       |\033[1;35m|       |\033[1;33m|       |\033[1;35m| |_____ \033[1;33m|       |\033[1;35m|   |___ \033[1;33m|   |    \033[1;35m|   |    \n"
	@printf "\033[1;35m|  _   | \033[1;33m|       |\033[1;35m|  _   | \033[1;33m|_     _|\033[1;35m|_____  |\033[1;33m|       |\033[1;35m|    ___|\033[1;33m|   |___ \033[1;35m|   |___ \n"
	@printf "\033[1;35m| |_|   |\033[1;33m|   _   |\033[1;35m| |_|   |\033[1;33m  |   |  \033[1;35m _____| |\033[1;33m|   _   |\033[1;35m|   |___ \033[1;33m|       |\033[1;35m|       |\n"
	@printf "\033[1;35m|_______|\033[1;33m|__| |__|\033[1;35m|_______|\033[1;33m  |___|  \033[1;35m|_______|\033[1;33m|__| |__|\033[1;35m|_______|\033[1;33m|_______|\033[1;35m|_______|\n\n\033[0m"

libft/libft.a:
	@printf "\033[2;36m\033[3;36m"
	$(MAKE) -sC libft all
	@printf "\e[0m"
	
obj/%.o:%.c
	@printf "\033[2;36m\033[3;36m"
	gcc -c $(CFLAGS) $< -o $@ -MD -MF $(@:obj/%.o=dep/%.d)
	@printf "\e[0m"

clean:
	@printf "\033[2;31m"
	$(MAKE) -sC libft fclean
	rm -f $(addprefix obj/,$(OBJ)) $(addprefix dep/,$(DEP))
	@printf "\e[0m"

fclean: clean
	@printf "\033[2;31m"
	$(MAKE) -sC libft fclean
	rm -f $(NAME)
	@printf "\e[0m"

re: fclean all

obj:
	mkdir obj
dep:
	mkdir dep
#"valgrind --suppressions=valgrind_filter.supp --leak-check=full --show-leak-kinds=all --trace-children=yes  --track-fds=yes"
