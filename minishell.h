/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Alia <Alia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:19:34 by anasr             #+#    #+#             */
/*   Updated: 2022/02/21 19:33:40 by Alia             ###   ########.fr       */
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

/* ----------------------- > >> Macros << < ----------------------- */

# define SINGLE_ARROW 1
# define DOUBLE_ARROW 2
# define ERROR_CODE -1

/* ----------------------- > >> Struct << < ----------------------- */

typedef struct s_parser_info
{
	char	*input_files_delimiters[OPEN_MAX];
	char	*output_files[OPEN_MAX];
	int		in_arrow_flag[OPEN_MAX];
	int		out_arrow_flag[OPEN_MAX];

	char	**words;
	char	*cmd_path;
	char	*cmd[OPEN_MAX];

	int		exit_code;
}	t_parser_info;

/* --------------------- > >> Prototypes << < --------------------- */

/* ------------ ** simple helpers ** ------------ */

void	free_array(char **array);
char	*ft_strndup(const char *s1, int n);
void	skip_isspaces(int *index, char *input);

/* ------------- ** command path ** ------------- */

char	*get_cmd_path(char *cmd);

/* ----------------- ** split ** ---------------- */

void	split_input(char *input, t_parser_info *p);

/* --------------- ** builtins ** --------------- */

void	env(char **env);
int		cd(char *path);

/* ---------- ** command execution ** ----------- */

void	execute_command(t_parser_info *p);

#endif