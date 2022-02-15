#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>       
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

/*------- > >> REDIRECTION << < -----------*/

# define SINGLE_ARROW 1
# define DOUBLE_ARROW 2


# define ERROR_CODE -1

typedef struct s_parser_info{
	char	*input_files[OPEN_MAX];
	char	*output_files[OPEN_MAX];
	char	*cmd_path;
	char	**cmd;
	int		exit_code;
	
	int		in_arrow_flag; //can be equal to SINGLE_ARROW or DOUBLE_ARROW based on last redirection
	int		out_arrow_flag;
	int		in_single_arrow_count;
	int		in_double_arrow_count;
	int		in_arrow_count;
	int		out_single_arrow_count;
	int		out_double_arrow_count;
	int		out_arrow_count;
} t_parser_info;


void	free_array(char **array);
char	*ft_strndup(const char *s1, int n);

char	*get_cmd_path(char **env, char *cmd);

#endif