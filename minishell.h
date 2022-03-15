/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:19:34 by anasr             #+#    #+#             */
/*   Updated: 2022/03/15 17:58:54 by anasr            ###   ########.fr       */
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

# define RED			"\033[0;31m"
# define BLUE			"\033[0;34m"
# define GREEN			"\033[0;32m"
# define YELLOW			"\033[0;33m"
# define PURPLE			"\033[0;35m"
# define LIGHT_BLUE		"\033[0;36m"
# define RESET			"\x1B[0m"

# define RED1			"\033[1;31m"
# define BLUE1			"\033[1;34m"
# define GREEN1			"\033[1;32m"
# define YELLOW1		"\033[1;33m"
# define PURPLE1		"\033[1;35m"
# define LIGHT_BLUE1	"\033[1;36m"

# define RED2			"\033[2;31m"
# define BLUE2			"\033[2;34m"
# define GREEN2			"\033[2;32m"
# define YELLOW2		"\033[2;33m"
# define PURPLE2		"\033[2;35m"
# define LIGHT_BLUE2	"\033[2;36m"

# define RED3			"\033[3;31m"
# define BLUE3			"\033[3;34m"
# define GREEN3			"\033[3;32m"
# define YELLOW3		"\033[3;33m"
# define PURPLE3		"\033[3;35m"
# define LIGHT_BLUE3	"\033[3;36m"

/* ----------------------- > >> Macros << < ----------------------- */

# define SINGLE_ARROW 1
# define DOUBLE_ARROW 2
# define ERROR_CODE -1

/* ----------------------- > >> Struct << < ----------------------- */

typedef struct s_parser_info
{
	char	**env;

	int		in_arrow_flag[255][255];
	char	*output_files[255][255];
	int		out_arrow_flag[255][255];
	char	*input_files_delimiters[255][255];

	// char	**cmd_part[255];
	// char	*cmd_path[255];
	// char	*cmd[255][255];
	char	***cmd;
	char	**cmd_path;
	char	***cmd_part;

	int		exit_code;
	bool	do_not_expand[255];

	int		pipes_count;
	char	**cmd_array;
}	t_parser_info;

/* ------------------ > >> Global variables << < ------------------ */

// extern char	**environ;

/* --------------------- > >> Prototypes << < --------------------- */

/* ------------- ** parser utils ** ------------- */

int		check_repeated_meta(char *input);

/* ------------ ** simple helpers ** ------------ */

void	clear(void);
char	**dup_array(char **a1);
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
char	**ft_split_custom(char *input, char **meta);

/* ------------ ** expand dollar ** ------------- */

char	*expand_dollars_in_str(char *str);
// char	*expand_dollar(char *str);

/* ----------- ** execution utils ** ------------ */

int		final_out_fd(int array_index, t_parser_info *p);
int		account_for_out_redirect(int i, t_parser_info *p);
int		final_in_fd(int array_index, t_parser_info *p, int pipe_end[2]);
int		account_for_in_redirect(int i, int *pipe_append, t_parser_info *p);
int		child_input_append(int array_index, t_parser_info *p, int i, int pipe_append[2]);

/* ---------- ** command execution ** ----------- */

void	execute_command(t_parser_info *p);
int		builtin_check(t_parser_info *p, int i);

/* ----------------- ** pipex ** ---------------- */

void	init_pipex(t_parser_info *p);

/* --------------- ** export env ** ------------- */

int		find_equal(char *env);
char	**export_env(char **env, char *env_variable);

/* ---------------- ** builtins ** -------------- */

int		pwd(void);
int		cd(char **argv);
int		echo(char **argv);
int		env(t_parser_info *p);
int		export(t_parser_info *p, char **cmd);

/* ----------------- ** getenv ** --------------- */

char	*local_getenv(char *var, char **p_env);

/* ------------------ ** pipe ** ---------------- */

void	pipe_stuff(t_parser_info *p);

#endif