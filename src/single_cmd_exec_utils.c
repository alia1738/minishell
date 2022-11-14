/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd_exec_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 13:05:46 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/04 13:48:50 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_n_exit(t_parser_info *p)
{
	free(p->cmd_path);
	free_everything(p);
	free_double_char(p->env);
	free_double_int(p->pipe_append, 1);
	exit(p->exit_code);
}

void	append_n_fds(t_parser_info *p)
{
	close(p->exit_code_fd[0]);
	write(p->exit_code_fd[1], "1", 1);
	do_in_append(p, 0, p->pipe_append[0]);
	signal(SIGINT, SIG_DFL);
	write(p->exit_code_fd[1], "30", 3);
	close(p->exit_code_fd[1]);
	p->in_fd = final_in_fd(0, p);
	if (p->in_fd == -1)
	{
		account_for_in_redirect(p->pipe_append[0], p->in_fd);
		free_everything(p);
		free_double_int(p->pipe_append, 1);
		free_double_char(p->env);
		exit(p->exit_code);
	}
	p->out_fd = final_out_fd(0, p);
	if (p->out_fd == -1)
	{
		account_for_in_redirect(p->pipe_append[0], p->in_fd);
		free_everything(p);
		free_double_int(p->pipe_append, 1);
		free_double_char(p->env);
		exit(p->exit_code);
	}
}

void	before_n_after_single_cmd(t_parser_info *p, bool if_before)
{
	if (if_before == true)
	{
		p->pipe_append = create_pipes(1);
		append_n_fds(p);
		if (builtin_check(p, 0) == 2 && p->cmd_absolute_path[0] == true)
			change_cmd(p, 0);
		else if (builtin_check(p, 0) == 2)
			p->cmd_path = get_cmd_path(p->cmd[0][0], p);
	}
	else
	{
		account_for_in_redirect(p->pipe_append[0], p->in_fd);
		if (p->out_fd > 1)
			close(p->out_fd);
		free_n_exit(p);
	}
}

void	account_for_in_redirect(int *pipe_append, int in_fd)
{	
	if (in_fd > 1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
		in_fd = 0;
	}
	if (in_fd == 1)
	{
		close(pipe_append[1]);
		dup2(pipe_append[0], STDIN_FILENO);
		close(pipe_append[0]);
	}
	else if (in_fd != 1)
	{
		close(pipe_append[1]);
		close(pipe_append[0]);
	}
}
