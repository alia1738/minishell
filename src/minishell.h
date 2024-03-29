/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:19:34 by anasr             #+#    #+#             */
/*   Updated: 2022/11/14 17:13:47 by aalsuwai         ###   ########.fr       */
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
# include "../libft/libft.h"
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

	char	***cmd;
	char	*cmd_path;
	char	***cmd_part;
	bool	*cmd_absolute_path;

	int		pipes_count;
	int		*child_pids;
	char	**cmd_array;

	int		**in_arrow_flag;
	char	***output_files;
	int		**out_arrow_flag;
	char	***input_files_delimiters;

	int		in_fd;
	int		out_fd;
	int		**pip;
	int		**pipe_append;
	int		exit_code_fd[2];

	bool	oldpwd_dont_update;

	bool	meta_pipe;
	bool	meta_place_taken;

	bool	was_there_delim;
	bool	command_in_action;
	bool	in_append_inprogress;

	bool	signal_in_cmd;

	int		exit_code;
}	t_parser_info;

typedef struct s_dollar_expansion
{
	int		i;
	int		new_index;
	char	*expanded;
	char	*meta[15];

	int		len;

}	t_dollar_expansion;

typedef struct s_sort_indices
{
	int	i;
	int	in_index;
	int	out_index;
	int	cmd_index;
}	t_sort_indices;

/* --------------------- > >> Prototypes << < --------------------- */

/* ------------- ** parser utils1 ** ------------ */

int				check_empty_input(char	*input);
int				check_repeated_meta(char *input, t_parser_info *p);

/* ------------- ** parser utils2 ** ------------ */

int				ft_isquote(char c);
char			*strcpy_wout_quotes(char *str);
int				ft_ismeta(char *current_c, char **meta);
int				skip_quote_content(int *i, char *input);

/* -------------- ** parse cmds ** -------------- */

void			save_cmds(char *input, t_parser_info *p);
void			save_input_output_files_n_cmds(int array_index, \
				char **specific_cmd, t_parser_info *p);

/* ------------- ** sort utils ** --------------- */

void			save_cmds_meta_init(char **meta);
void			save_cmds_helper(int array_index, char *input, \
				char **meta, t_parser_info *p);
void			sort_input(int array_index, char **specific_cmd, \
				t_sort_indices *index, t_parser_info *p);
void			sort_output(int array_index, char **specific_cmd, \
				t_sort_indices *index, t_parser_info *p);
void			sort_cmds(int array_index, char **specific_cmd, \
				t_sort_indices *index, t_parser_info *p);

/* ------------ ** simple helpers ** ------------ */

char			**dup_array(char **a1);
int				clear(t_parser_info *p);
char			*ft_strndup(const char *s1, int n);
void			skip_isspaces(int *index, char *input);
char			*ft_strcpy(char *dst, const char *src);

int				check_longmax(char *str);
int				ft_str_isdigit(char *str);
char			*ft_str_tolower(char *str);
int				compare_caseless(const char *s_unknown, \
				const char *s_lowercase);

void			*ft_calloc_p(size_t count, size_t size);
int				ft_smartncmp(const char *s1, const char *s2, size_t len);

/* ------------- ** command path ** ------------- */

void			nested_minishell(t_parser_info *p);
void			change_cmd(t_parser_info *p, int array_i);
char			*get_cmd_path(char *cmd, t_parser_info *p);

/* ----------------- ** split ** ---------------- */

void			count_inputs_helper(int *i, char *input, char **meta);

char			**ft_split_custom(char *input, char **meta, t_parser_info *p);

/* ------------ ** expand dollar ** ------------- */

void			skip_dollar_content(int *i, char *str);
char			*ft_getenv(int i, char *str, t_parser_info *p);
char			*expand_dollars_in_str(char *str, t_parser_info *p, \
				bool append_flag);

/* ---------- ** expand dollar utils1 ** --------- */

void			increment_len_n_index(t_dollar_expansion *dxl);
void			len_dollar_question(t_dollar_expansion *dxl, \
				t_parser_info *p);
void			len_dollar_general(t_dollar_expansion *dxl, char *str, \
				t_parser_info *p);
void			len_dollar_quote(int type, char *str, t_dollar_expansion *dxl, \
				t_parser_info *p);
void			len_dollar_single_quote(bool append_flag, char *str, \
				t_dollar_expansion *dxl, t_parser_info *p);

/* ---------- ** expand dollar utils2 ** --------- */

void			expand_dollar_question(t_dollar_expansion *dx, \
				t_parser_info *p);
void			expand_dollar_general(char *str, t_dollar_expansion *dx, \
				t_parser_info *p);
void			expand_dollar_quote(int type, char *str, \
				t_dollar_expansion *dx, t_parser_info *p);
void			expand_dollar_single_quote(bool append_flag, char *str, \
				t_dollar_expansion *dx, t_parser_info *p);

/* ----------- ** single cmd exec ** ------------ */

void			execute_single_command(t_parser_info *p);

/* ------------ single cmd exec utils ----------- */

void			free_n_exit(t_parser_info *p);
void			append_n_fds(t_parser_info *p);
void			account_for_in_redirect(int *pipe_append, int in_fd);
void			before_n_after_single_cmd(t_parser_info *p, bool if_before);

/* --------------- ** pipe exec ** -------------- */

void			execute_pipe_execution(t_parser_info *p);

/* ------------- ** pipe exec utils ** ---------- */

int				**create_pipes(int size);
int				**create_pipe_append(int size);
void			get_in_out_fds(t_parser_info *p, int **pipe_append);
void			check_out_fd(int in_fd, int **pip, int i, int pipe_count);
void			check_in_fd(int in_fd, int *pipe_append, int **pip, int i);

/* ----------- ** pipe exec utils 2 ** ---------- */

void			free_close_exit(t_parser_info *p);
void			close_all_pipes(int **pip, int len);
void			close_all_pipes_fds(t_parser_info *p);
void			before_command(t_parser_info *p, int i);
void			close_pip_append(t_parser_info *p, int pip_i);
void			close_remaining_pipes(t_parser_info *p, int i, int max);

/* --------------- ** append ** ---------------- */

void			big_baby_job(t_parser_info *p);
void			make_append_child(t_parser_info *p);
void			do_in_append(t_parser_info *p, int array_i, int pipe_end[2]);
int				child_input_append(int array_index, t_parser_info *p, int i, \
				int pipe_append[2]);

/* ------------- ** redirection ** -------------- */

int				final_in_fd(int array_index, t_parser_info *p);
int				final_out_fd(int array_index, t_parser_info *p);

/* ------------ ** export env utils ** ---------- */

int				find_equal(char *env);
bool			append_env_check(char *new_env_var);
int				get_index(char *env_variable, char **env);
int				error_check(t_parser_info *p, char *new_var, char *type);

/* --------------- ** export env ** ------------- */

char			**export_env(t_parser_info *p, char **env, char *new_env_var);

/* --------------- ** unset env ** -------------- */

char			**unset_env(t_parser_info *p, char **env, \
				char *to_be_removed_var);

/* ---------------- ** builtins ** -------------- */

int				builtin_check(t_parser_info *p, int i);
int				builtin_execute(t_parser_info *p, int i);
int				unset(t_parser_info *p, char **cmd);
int				export(t_parser_info *p, char **cmd);

int				pwd(t_parser_info *p);
int				cd(t_parser_info *p, char **argv);

int				env(t_parser_info *p);
int				echo(t_parser_info *p, char **argv);
void			baby_exit(t_parser_info *p, char **cmd);

/* ----------------- ** getenv ** --------------- */

char			*local_getenv(char *var, char **p_env);

/* -------------- ** free utils ** -------------- */

void			free_double_int(int **array, int rows);
void			free_double_char(char **array);
void			free_triple_char(char ***array);
void			free_triple_char_partial(char ***array, t_parser_info *p);

/* ------------ ** counting utils ** ------------ */

int				count_pipes(char *input);
int				count_cmds_wout_meta(char *str, t_parser_info *p);
int				count_in_redirections(char *str);
int				count_out_redirections(char *str);

/* ----------------- ** memory ** --------------- */

void			free_everything(t_parser_info *p);
void			allocate_meme_general(t_parser_info *p);
void			allocate_meme_specific(char *str, int array_index, \
				t_parser_info *p);

/* ----------------- ** signals ** -------------- */

void			hide_signal_markers(void);
void			handle_signals(int signum);
void			append_handler(int signum);
t_parser_info	*return_p(t_parser_info *p);

#endif