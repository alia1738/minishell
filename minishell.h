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

# define ONE_IN_ONE_OUT 1 
# define TWO_IN_ONE_OUT 2 
# define ONE_IN_TWO_OUT 3 
# define TWO_IN_TWO_OUT 4 

# define SINGLE_ARROW 5
# define DOUBLE_ARROW 6


# define ERROR_CODE -1

typedef struct s_parser_info{
	char	*input_files[OPEN_MAX];
	char	*output_files[OPEN_MAX];
	char	*cmd_path;
	char	**cmd;
	
	int		arrow_flag;
	int		in_arrow_flag;
	int		out_arrow_flag;
	int		in_single_arrow_count;
	int		in_double_arrow_count;
	int		out_single_arrow_count;
	int		out_double_arrow_count;
} t_parser_info;


void	free_array(char **array);
char	*ft_strndup(const char *s1, int n);

char	*get_cmd_path(char **env, char *cmd);

#endif