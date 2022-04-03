/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:25:34 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/03 16:47:23 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pip_append(t_parser_info *p, int pip_i)
{
	int	i;

	i = -1;
	while (++i <= p->pipes_count)
	{
		if (i != p->pipes_count && i != (pip_i - 1) && i != pip_i)
		{
			close(p->pip[i][0]);
			close(p->pip[i][1]);
		}
		if (i != pip_i)
		{
			close(p->pipe_append[i][0]);
			close(p->pipe_append[i][1]);
		}
	}
}

void	close_all_pipes_fds(t_parser_info *p)
{
	int	i;

	i = -1;
	while (++i <= p->pipes_count)
	{
		if (i != p->pipes_count)
		{
			close(p->pip[i][0]);
			close(p->pip[i][1]);
		}
		close(p->pipe_append[i][0]);
		close(p->pipe_append[i][1]);
	}
}

void	close_remaining_pipes(t_parser_info *p, int i, int max)
{
	if (i < max)
	{	
		close(p->pip[i][0]);
		close(p->pip[i][1]);
	}
	if (i)
	{
		close(p->pip[i - 1][0]);
		close(p->pip[i - 1][1]);
	}
	close(p->pipe_append[i][0]);
	close(p->pipe_append[i][1]);
}

void	free_close_exit(t_parser_info *p)
{
	if (p->in_fd > 1)
		close(p->in_fd);
	if (p->out_fd > 1)
		close(p->out_fd);
	free(p->cmd_path);
	free_everything(p);
	free_double_char(p->env);
	free_double_int(p->pip, p->pipes_count);
	free_double_int(p->pipe_append, (p->pipes_count + 1));
	exit(p->exit_code);
}

void	before_command(t_parser_info *p, int i)
{
	close_pip_append(p, i);
	p->in_fd = final_in_fd(i, p);
	if (p->in_fd == -1)
	{
		close_remaining_pipes(p, i, p->pipes_count);
		free_close_exit(p);
	}
	p->out_fd = final_out_fd(i, p);
	if (p->out_fd == -1)
	{
		close_remaining_pipes(p, i, p->pipes_count);
		free_close_exit(p);
	}
	if (!p->cmd[i][0])
	{
		p->exit_code = 1;
		close_remaining_pipes(p, i, p->pipes_count);
		free_close_exit(p);
	}
	if (builtin_check(p, i) == 2 && p->cmd_absolute_path[i] == true)
		change_cmd(p, i);
	else if (builtin_check(p, i) == 2)
		p->cmd_path = get_cmd_path(p->cmd[i][0], p);
	check_in_fd(p->in_fd, p->pipe_append[i], p->pip, i);
	check_out_fd(p->out_fd, p->pip, i, p->pipes_count);
}
