/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:19:34 by anasr             #+#    #+#             */
/*   Updated: 2022/03/22 13:49:40 by aalsuwai         ###   ########.fr       */
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

	// int		*in_fds;
	// int		*out_fds;
	int		**in_arrow_flag;
	char	***output_files;
	int		**out_arrow_flag;
	char	***input_files_delimiters;

	int		exit_code;
}	t_parser_info;

/* ------------------ > >> Global variables << < ------------------ */

// extern char	**environ;
extern int rl_catch_signals;

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
int		final_in_fd(int array_index, t_parser_info *p, int pipe_end[2]);
void	account_for_in_redirect(int *pipe_append, t_parser_info *p, int in_fd);
int		child_input_append(int array_index, t_parser_info *p, int i, int pipe_append[2]);

/* ---------- ** command execution ** ----------- */

void	execute_command(t_parser_info *p);
int		builtin_execute(t_parser_info *p, int i);

/* --------------- ** export env ** ------------- */

int		find_equal(char *env);
char	**export_env(char **env, char *env_variable);

/* ---------------- ** builtins ** -------------- */

int		pwd(void);
int		cd(char **argv);
int		echo(char **argv);
int		env(t_parser_info *p);
int		export(t_parser_info *p, char **cmd);
void	baby_exit(t_parser_info *p, char **cmd);

/* ----------------- ** getenv ** --------------- */

char	*local_getenv(char *var, char **p_env);

/* ------------------ ** pipe ** ---------------- */

void	pipe_stuff(t_parser_info *p);

/* --------------- ** pipe utils ** ------------- */

int		**create_pipes(t_parser_info *p);
int		**create_pipe_append(t_parser_info *p);
void	get_in_out_fds(t_parser_info *p, int **pipe_append);
void	check_out_fd(int in_fd, int **pip, int i, int pipe_count);
void	check_in_fd(int in_fd, int *pipe_append, int **pip, int i);

/* ------------- ** pipe utils 2 ** ------------- */

int		pipe_final_in_fd(int array_i, t_parser_info *p);
int		pipe_final_out_fd(int array_i, t_parser_info *p);
void	close_all_pipes_fds(t_parser_info *p, int **pip, int **pipe_append);
void	close_pip_append(t_parser_info *p,int **pip, int **append, int pip_i);

/* ------------- ** pipe append ** -------------- */

void	big_baby_job(t_parser_info *p, int **pipe_append);
void	make_append_child(t_parser_info *p, int **pipe_append);

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