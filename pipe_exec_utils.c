/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 19:14:46 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/04 14:36:49 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**create_pipes(int size)
{
	int	i;
	int	**pip;

	i = -1;
	pip = ft_calloc(size, sizeof(int *));
	while (++i < size)
	{
		pip[i] = ft_calloc(2, sizeof(int));
		if 	(pipe(pip[i]) < 0)
			exit(1);
	}
	return (pip);
}

static void	check_in_fd_helper(int in_fd, int *pipe_append, int **pip, int i)
{
	if (in_fd != 1)
	{
		close(pipe_append[1]);
		close(pipe_append[0]);
	}
	if (in_fd && i)
	{
		close(pip[i - 1][0]);
		close(pip[i - 1][1]);
	}
}

void	check_in_fd(int in_fd, int *pipe_append, int **pip, int i)
{
	if (in_fd > 1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
		in_fd = 0;
	}
	else if (in_fd == 1)
	{
		close(pipe_append[1]);
		dup2(pipe_append[0], STDIN_FILENO);
		close(pipe_append[0]);
	}
	else if (!in_fd && i)
	{
		close(pip[i - 1][1]);
		dup2(pip[i - 1][0], STDIN_FILENO);
		close(pip[i - 1][0]);
	}
	check_in_fd_helper(in_fd, pipe_append, pip, i);
}

void	check_out_fd(int out_fd, int **pip, int i, int pipe_count)
{
	if (out_fd > 1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
		out_fd = 0;
	}
	else if (!out_fd && i != pipe_count)
	{
		close(pip[i][0]);
		dup2(pip[i][1], STDOUT_FILENO);
		close(pip[i][1]);
	}
	if (out_fd && i != pipe_count)
	{
		close(pip[i][0]);
		close(pip[i][1]);
	}
}
