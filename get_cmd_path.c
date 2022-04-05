/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 16:55:09 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 16:47:54 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	nested_minishell(t_parser_info *p)
{
	int		save;
	char	*temp;
	char	*shlvl;

	temp = local_getenv("SHELL", p->env);
	if (temp)
		export_env(p, p->env, "SHELL=minishell");
	temp = local_getenv("SHLVL", p->env);
	if (temp)
	{
		save = ft_atoi(temp);
		++save;
		temp = ft_itoa(save);
		shlvl = ft_strjoin("SHLVL=", temp);
		free(temp);
		export_env(p, p->env, shlvl);
		free(shlvl);
	}
}

void	change_cmd(t_parser_info *p, int array_i)
{
	if (access(p->cmd[array_i][0], F_OK) == -1)
	{
		printf("babyshell: %s: No such file or directory\n", p->cmd[array_i][0]);
		p->exit_code = 127;
	}
	else if (access(p->cmd[array_i][0], X_OK) == -1)
	{
		printf("babyshell: %s: Permission denied\n", p->cmd[array_i][0]);
		p->exit_code = 126;
	}
	else
	{
		p->cmd_path = ft_strdup(p->cmd[array_i][0]);
		free(p->cmd[array_i][0]);
		p->cmd[array_i][0] = ft_strdup(ft_strrchr(p->cmd_path, '/') + 1);
	}
}

static char	*join_cmd_to_path(char *path, char *cmd)
{
	char	*temp;
	char	*joined;

	temp = ft_strjoin(path, "/");
	joined = ft_strjoin(temp, cmd);
	free(temp);
	return (joined);
}

static void	perror_n_exitcode(char *cmd, t_parser_info *p, bool no_cmd)
{
	p->exit_code = 127;
	if (no_cmd == false)
		printf("babyshell: %s: No such file or directory\n", cmd);
	else
		printf("babyshell: %s:  command not found\n", cmd);
}

char	*get_cmd_path(char *cmd, t_parser_info *p)
{
	int		i;
	char	*temp_path;
	char	**paths_array;

	i = -1;
	if (!cmd)
		return (NULL);
	temp_path = local_getenv("PATH", p->env);
	if (!temp_path)
		perror_n_exitcode(cmd, p, false);
	if (!temp_path)
		return (0);
	paths_array = ft_split(temp_path, ':');
	while (paths_array[++i] && cmd[0])
	{
		temp_path = join_cmd_to_path(paths_array[i], cmd);
		if (!access(temp_path, F_OK) && !access(temp_path, X_OK))
			free_double_char(paths_array);
		if (!access(temp_path, F_OK) && !access(temp_path, X_OK))
			return (temp_path);
		free(temp_path);
	}
	perror_n_exitcode(cmd, p, true);
	free_double_char(paths_array);
	return (NULL);
}
