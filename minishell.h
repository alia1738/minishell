/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:19:34 by anasr             #+#    #+#             */
/*   Updated: 2022/03/20 12:51:57 by anasr            ###   ########.fr       */
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
# include <signal.h>
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

# define ERROR_CODE -1
# define SINGLE_ARROW 1
# define DOUBLE_ARROW 2

/* ----------------------- > >> Struct << < ----------------------- */

typedef struct s_parser_info
{
	char	**env;

	char	***cmd;
	char	**cmd_path;
	char	***cmd_part;

	int		pipes_count;
	int		*child_pids;
	char	**cmd_array;

	int		**in_arrow_flag;
	char	***output_files;
	int		**out_arrow_flag;
	char	***input_files_delimiters;

	int		exit_code;
}	t_parser_info;

/* ------------------ > >> Global variables << < ------------------ */

// extern char	**environ;
// extern int rl_catch_signals;

/* --------------------- > >> Prototypes << < --------------------- */

/* ------------- ** parser utils ** ------------- */

int		check_repeated_meta(char *input);

/* ------------ ** simple helpers ** ------------ */

int		clear(void);
char	**dup_array(char **a1);
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
int		error_check(t_parser_info *p, char *new_var);
char	**export_env(t_parser_info *p, char **env, char *env_variable);

/* --------------- ** unset env ** -------------- */

char	**unset_env(t_parser_info *p, char **env, char *to_be_removed_var);

/* ---------------- ** builtins ** -------------- */

int		pwd(t_parser_info *p);
int		cd(t_parser_info *p, char **argv);
int		echo(t_parser_info *p, char **argv);
int		env(t_parser_info *p);
int		export(t_parser_info *p, char **cmd);
int		unset(t_parser_info *p, char **cmd);
void	baby_exit(t_parser_info *p, char **cmd);

/* ----------------- ** getenv ** --------------- */

char	*local_getenv(char *var, char **p_env);

/* ------------------ ** pipe ** ---------------- */

void	pipe_stuff(t_parser_info *p);

/* -------------- ** free utils ** -------------- */

void	free_double_char(char **array);
void	free_triple_char(char ***array);
void	free_double_int(int **array, int rows);
void	free_triple_char_partial(char ***array);

/* ------------ ** counting utils ** ------------ */

int		count_pipes(char *input);
int		count_cmds_wout_meta(char *str);
int		count_in_redirections(char	*str);
int		count_out_redirections(char	*str);

/* ----------------- ** memory ** --------------- */

void	free_everything(t_parser_info *p);
void	allocate_meme_general(t_parser_info *p);
void	allocate_meme_specific(char *str, int array_index,t_parser_info *p);


void	handle_signals(int signum);

#endif