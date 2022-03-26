/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Alia <Alia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 19:14:46 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/26 16:42:18 by Alia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**create_pipes(t_parser_info *p)
{
	int	i;
	int	**pip;

	i = -1;
	pip = ft_calloc((p->pipes_count), sizeof(int *));
	while (++i < p->pipes_count)
	{
		pip[i] = ft_calloc(2, sizeof(int));
		pipe(pip[i]);
	}
	return (pip);
}

int	**create_pipe_append(t_parser_info *p)
{
	int	i;
	int	**pipe_append;

	i = -1;
	pipe_append = ft_calloc((p->pipes_count + 1), sizeof(int *));
	while (++i <= p->pipes_count)
	{
		pipe_append[i] = ft_calloc(2, sizeof(int));
		pipe(pipe_append[i]);
	}
	return (pipe_append);
}

void	check_in_fd(int in_fd, int *pipe_append, int **pip, int i)
{
	if (in_fd > 1)
		dup2(in_fd, STDIN_FILENO);
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

void	check_out_fd(int out_fd, int **pip, int i, int pipe_count)
{
	if (out_fd > 1)
		dup2(out_fd, STDOUT_FILENO);
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
