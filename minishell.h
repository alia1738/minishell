/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:19:34 by anasr             #+#    #+#             */
/*   Updated: 2022/03/03 12:09:23 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ----------------------- > >> Headers << < ---------------------- */

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <sys/types.h>
# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>

/* ----------------------- > >> Colors << < ----------------------- */

# define RED		"\x1B[31m"
# define BLUE		"\x1B[34m"
# define GREEN		"\x1B[32m"
# define YELLOW		"\x1B[33m"
# define MAGENTA	"\x1B[35m"
# define LIGHT_BLUE	"\x1B[36m"
# define RESET		"\x1B[0m"

/* ----------------------- > >> Macros << < ----------------------- */

# define SINGLE_ARROW 1
# define DOUBLE_ARROW 2
# define ERROR_CODE -1

/* ----------------------- > >> Struct << < ----------------------- */

typedef struct s_parser_info
{
	char	**env;
	
	char	*input_files_delimiters[FOPEN_MAX][FOPEN_MAX];
	char	*output_files[FOPEN_MAX][FOPEN_MAX];
	int		in_arrow_flag[FOPEN_MAX][FOPEN_MAX];
	int		out_arrow_flag[FOPEN_MAX][FOPEN_MAX];

	char	**words[FOPEN_MAX];
	char	*cmd_path[FOPEN_MAX];
	char	*cmd[FOPEN_MAX][FOPEN_MAX];
	int		word_index;

	int		exit_code;
	bool	do_not_expand[FOPEN_MAX];

	//pipex
	int		pipes_count;
	char	**cmd_array;
}	t_parser_info;

/* --------------------- > >> Prototypes << < --------------------- */

/* ------------ ** simple helpers ** ------------ */

void	clear(void);
void	free_double(char **array);
void	free_triple(char ***array);
char	*ft_strndup(const char *s1, int n);
void	skip_isspaces(int *index, char *input);
char	*ft_strcpy(char *dst, const char *src);

/* ------------- ** command path ** ------------- */

char	*get_cmd_path(char *cmd);

/* ----------------- ** split ** ---------------- */

char	**ft_split_custom(char *input, char **meta, t_parser_info *p);

/* ---------- ** command execution ** ----------- */

void	execute_command(t_parser_info *p);

/* ----------- ** execution utils ** ------------ */

int		final_out_fd(int array_index, t_parser_info *p);
int		final_in_fd(int array_index, t_parser_info *p, int pipe_end[2]);
int		child_input_append(int array_index, t_parser_info *p, int i, int pipe_append[2]);

/* ------------ ** expand dollar ** ------------- */

char	*expand_dollar(char *str);

/* ----------------- ** pipex ** ---------------- */

void	init_pipex(t_parser_info *p);

#endif