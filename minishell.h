/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:19:34 by anasr             #+#    #+#             */
/*   Updated: 2022/04/01 17:01:06 by aalsuwai         ###   ########.fr       */
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
	char	*input;

	char	**env;

	bool	*cmd_absolute_path;
	char	***cmd;
	char	*cmd_path;
	char	***cmd_part;

	int		pipes_count;
	int		*child_pids;
	char	**cmd_array;

	int		**in_arrow_flag;
	char	***output_files;
	int		**out_arrow_flag;
	char	***input_files_delimiters;

	int		in_fd;
	int		out_fd;

	bool	oldpwd_dont_update;

	bool	in_append_inprogress;
	bool	command_in_action;
	bool	was_there_delim;

	bool	signal_in_cmd;

	int		exit_code;
}	t_parser_info;

/* ------------------ > >> Global variables << < ------------------ */

// extern char	**environ;
extern int rl_catch_signals;

/* --------------------- > >> Prototypes << < --------------------- */

/* ------------- ** parser utils ** ------------- */

int		check_repeated_meta(char *input, t_parser_info *p);

/* ------------ ** simple helpers ** ------------ */

char	**dup_array(char **a1);
int		clear(t_parser_info *p);
char	*ft_strndup(const char *s1, int n);
void	skip_isspaces(int *index, char *input);
char	*ft_strcpy(char *dst, const char *src);
int		ft_str_isdigit(char *str);
char	*ft_str_tolower(char *str);
int		compare_caseless(const char *s_unknown, const char *s_lowercase);
int		check_longmax(char *str);

/* ------------- ** command path ** ------------- */

void	change_cmd(t_parser_info *p, int array_i);
char	*get_cmd_path(char *cmd, t_parser_info *p);

/* ----------------- ** split ** ---------------- */

int		ft_isquote(char c);
char	*strcpy_wout_quotes(char *str);
int		ft_ismeta(char *current_c, char **meta);
int		skip_quote_content(int *i, char *input);
char	**ft_split_custom(char *input, char **meta, t_parser_info *p);

/* ------------ ** expand dollar ** ------------- */

void	skip_dollar_content(int *i, char *str);
char	*ft_getenv(int i, char *str, t_parser_info *p);
char	*expand_dollars_in_str(char *str, t_parser_info *p, bool append_flag);

/* ---------- ** expand dollar utils ** --------- */


void	len_dollar_question(int *i, int *len, t_parser_info *p);
void	expand_dollar_question(int *i, int *new_index, char *expanded, t_parser_info *p);
void	len_dollar_general(int *i, int *len, char *str, t_parser_info *p);

/* ----------- ** execution utils ** ------------ */

int		final_in_fd(int array_index, t_parser_info *p);
int		final_out_fd(int array_index, t_parser_info *p);
void	account_for_in_redirect(int *pipe_append, int in_fd);
void	do_in_append(t_parser_info *p, int array_i, int pipe_end[2]);
int		child_input_append(int array_index, t_parser_info *p, int i, int pipe_append[2]);

/* ---------- ** command execution ** ----------- */

void	execute_single_command(t_parser_info *p);
int		builtin_check(t_parser_info *p, int i);
int		builtin_execute(t_parser_info *p, int i);

/* --------------- ** export env ** ------------- */

int		find_equal(char *env);
int		error_check(t_parser_info *p, char *new_var);
char	**export_env(t_parser_info *p, char **env, char *new_env_var);

/* --------------- ** unset env ** -------------- */

char	**unset_env(t_parser_info *p, char **env, char *to_be_removed_var);

/* ---------------- ** builtins ** -------------- */

int		pwd(t_parser_info *p);
int		env(t_parser_info *p);
int		cd(t_parser_info *p, char **argv);
int		echo(t_parser_info *p, char **argv);
int		unset(t_parser_info *p, char **cmd);
int		export(t_parser_info *p, char **cmd);
void	baby_exit(t_parser_info *p, char **cmd);

/* ----------------- ** getenv ** --------------- */

char	*local_getenv(char *var, char **p_env);

/* ------------------ ** pipe ** ---------------- */

void	execute_pipe_execution(t_parser_info *p);

/* --------------- ** pipe utils ** ------------- */

int		**create_pipes(t_parser_info *p);
int		**create_pipe_append(t_parser_info *p);
void	get_in_out_fds(t_parser_info *p, int **pipe_append);
void	check_out_fd(int in_fd, int **pip, int i, int pipe_count);
void	check_in_fd(int in_fd, int *pipe_append, int **pip, int i);

/* ------------- ** pipe utils 2 ** ------------- */

void	free_n_close(t_parser_info *p, int **pip, int **pipe_append);
void	close_all_pipes_fds(t_parser_info *p, int **pip, int **pipe_append);
void	close_remaining_pipes(int **pipe_append, int **pip, int i, int max);
void	before_command(t_parser_info *p, int **pip, int **pipe_append, int i);
void	close_pip_append(t_parser_info *p,int **pip, int **append, int pip_i);

/* ------------- ** pipe append ** -------------- */

void	big_baby_job(t_parser_info *p, int **pipe_append);
void	make_append_child(t_parser_info *p, int **pipe_append);

/* -------------- ** free utils ** -------------- */

void	free_double_int(int **array, int rows);
void	free_double_char(char **array);
void	free_triple_char(char ***array);
void	free_triple_char_partial(char ***array, t_parser_info  *p);

/* ------------ ** counting utils ** ------------ */

int		count_pipes(char *input);
int		count_cmds_wout_meta(char *str, t_parser_info *p);
int		count_in_redirections(char	*str);
int		count_out_redirections(char	*str);

/* ----------------- ** memory ** --------------- */

void	free_everything(t_parser_info *p);
void	allocate_meme_general(t_parser_info *p);
void	allocate_meme_specific(char *str, int array_index,t_parser_info *p);


void	handle_signals(int signum);

#endif