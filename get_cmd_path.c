/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 16:55:09 by anasr             #+#    #+#             */
/*   Updated: 2022/03/25 21:06:21 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		p->cmd_path[array_i] = ft_strdup(p->cmd[array_i][0]);
		free(p->cmd[array_i][0]);
		p->cmd[array_i][0] = ft_strdup(ft_strrchr(p->cmd_path[array_i], '/') + 1);
	}
}

char	*join_cmd_to_path(char *path, char *cmd)
{
	char	*temp;
	char	*joined;

	temp = ft_strjoin(path, "/");
	joined = ft_strjoin(temp, cmd);
	free(temp);
	return (joined);
}

/*Malloc occurs here*/

char	*get_cmd_path(char *cmd, t_parser_info *p)
{
	int		i;
	char	**paths_array;
	char	*temp_path;

	i = 0;
	if (!cmd)
		return (NULL);
	temp_path = local_getenv("PATH", p->env);
	if (!temp_path)
	{
		printf("babyshell: %s: No such file or directory\n", cmd);
		p->exit_code = 127;
		return (0);
	}
	paths_array = ft_split(temp_path, ':');
	while (paths_array[i])
	{
		temp_path = join_cmd_to_path(paths_array[i], cmd);
		if (!access(temp_path, F_OK))
			return (temp_path);
		free(temp_path);
		i++;
	}
	printf("minishell: %s:  command not found\n", cmd);
	free_double_char(paths_array);
	return (NULL);
	// exit (127); // error message
}
