/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 16:55:09 by anasr             #+#    #+#             */
/*   Updated: 2022/03/02 11:51:02 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
char	*get_cmd_path(char *cmd)
{
	int		i;
	char	**paths_array;
	char	*temp_path;

	i = 0;
	if (!ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "cd", 3) || \
	!ft_strncmp(cmd, "pwd", 4) || !ft_strncmp(cmd, "export", 7) || \
	!ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "env", 4) || \
	!ft_strncmp(cmd, "exit", 5) || !ft_strncmp(cmd, "minishell", 10))
	{
		temp_path = ft_strjoin("/Users/aalsuwai/Desktop/minishell/builtins/", cmd);
		return(temp_path);
	}
	if (!cmd)
		return (NULL);
	temp_path = getenv("PATH");
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
	free_array(paths_array);
	return (NULL);
	// exit (127); // error message
}
