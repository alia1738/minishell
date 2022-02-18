#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>       
# include <dirent.h>
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
	int		in_arrow_flag[OPEN_MAX];
	int		out_arrow_flag[OPEN_MAX];
	char	*cmd_path;
	char	**words;
	char	*cmd[OPEN_MAX];
	int		exit_code;
} t_parser_info;


void	free_array(char **array);
char	*ft_strndup(const char *s1, int n);
void	skip_isspaces(int *index, char *input);

char	*get_cmd_path(char **env, char *cmd);

void	split_input(char *input, t_parser_info *p);

#endif