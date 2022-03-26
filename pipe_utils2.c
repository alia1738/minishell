/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Alia <Alia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:25:34 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/26 16:44:50 by Alia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pip_append(t_parser_info *p, int **pip, int **append, int pip_i)
{
	int	i;

	i = -1;
	while (++i <= p->pipes_count)
	{
		if (i != p->pipes_count && i != (pip_i - 1) && i != pip_i)
		{
			close(pip[i][0]);
			close(pip[i][1]);
		}
		if (i != pip_i)
		{
			close(append[i][0]);
			close(append[i][1]);
		}
	}
}

void	close_all_pipes_fds(t_parser_info *p, int **pip, int **pipe_append)
{
	int	i;

	i = -1;
	while (++i <= p->pipes_count)
	{
		if (i != p->pipes_count)
		{
			close(pip[i][0]);
			close(pip[i][1]);
		}
		close(pipe_append[i][0]);
		close(pipe_append[i][1]);
	}
}

void	close_remaining_pipes(int **pipe_append, int **pip, int i, int max)
{
	if (i < max)
	{	
		close(pip[i][0]);
		close(pip[i][1]);
	}
	if (i)
	{
		close(pip[i - 1][0]);
		close(pip[i - 1][1]);
	}
	close(pipe_append[i][0]);
	close(pipe_append[i][1]);
}

void	free_n_close(t_parser_info *p, int **pip, int **pipe_append)
{
	if (p->in_fd > 0)
		close(p->in_fd);
	if (p->out_fd > 0)
		close(p->out_fd);
	free(p->env);
	free_everything(p);
	free_double_char(p->cmd_path);
	free_double_int(pip, p->pipes_count);
	free_double_int(pipe_append, (p->pipes_count + 1));
}

void	before_command(t_parser_info *p, int **pip, int **pipe_append, int i)
{
	p->in_fd = final_in_fd(i, p);
	p->out_fd = final_out_fd(i, p);
	if (!p->cmd[i][0] || p->in_fd == -1 || p->out_fd == -1)
	{
		close_all_pipes_fds(p, pip, pipe_append);
		free_n_close(p, pip, pipe_append);
		exit(1);
	}
	close_pip_append(p, pip, pipe_append, i);
	check_in_fd(p->in_fd, pipe_append[i], pip, i);
	check_out_fd(p->out_fd, pip, i, p->pipes_count);
}
