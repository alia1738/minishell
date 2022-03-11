/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:19:34 by anasr             #+#    #+#             */
/*   Updated: 2022/03/11 11:31:21 by anasr            ###   ########.fr       */
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

	char	*input_files_delimiters[255][255];
	char	*output_files[255][255];
	int		in_arrow_flag[255][255];
	int		out_arrow_flag[255][255];

	char	**words[255];
	char	*cmd_path[255];
	char	*cmd[255][255];
	int		word_index;

	int		exit_code;
	bool	do_not_expand[255];

	int		pipes_count;
	char	**cmd_array;
}	t_parser_info;

/* ------------------ > >> Global variables << < ------------------ */

extern char	**environ;

/* --------------------- > >> Prototypes << < --------------------- */

/* ------------- ** parser utils ** ------------- */

int		check_repeated_meta(char *input);

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

int		ft_isquote(char c);
int		ft_ismeta(char *current_c, char **meta);
int		skip_quote_content(int *i, char *input);
char	**ft_split_custom(char *input, char **meta, t_parser_info *p);

/* ------------ ** expand dollar ** ------------- */

char	*expand_dollars_in_str(char *str);
// char	*expand_dollar(char *str);

/* ----------- ** execution utils ** ------------ */

int		final_out_fd(int array_index, t_parser_info *p);
int		final_in_fd(int array_index, t_parser_info *p, int pipe_end[2]);
int		child_input_append(int array_index, t_parser_info *p, int i, int pipe_append[2]);
int		account_for_in_redirect(int i, int *pipe_append, int *in_pipe, t_parser_info *p);
int		account_for_out_redirect(int i, int *out_pipe, t_parser_info *p);

/* ---------- ** command execution ** ----------- */

void	execute_command(t_parser_info *p);

/* ----------------- ** pipex ** ---------------- */

void	init_pipex(t_parser_info *p);

/* --------------- ** export env ** ------------- */

void	export_env(char	**env, char *env_variable);

/* ---------------- ** builtins ** -------------- */

int		env(void);
int		pwd(void);
int		cd(char **argv);
int		echo(char **argv);

#endif