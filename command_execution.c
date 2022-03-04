/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/04 14:18:59 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_job(t_parser_info *p)
{
	int	in_fd;
	int	out_fd;
	int	pipe_append[2];

	pipe(pipe_append);
	in_fd = final_in_fd(0, p, pipe_append);
	out_fd = final_out_fd(0, p);
	account_for_in_redirect(0, pipe_append, 0, p);
	account_for_out_redirect(0, 0, p);
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
	//free here
	if (in_fd == 1)
		close(pipe_append[0]);
	else if (in_fd)
		close(in_fd);
	if (out_fd)
		close(out_fd);
	exit(127);
}

void	execute_command(t_parser_info *p)
{
	pid_t	child;

	child = fork();
	if (!child)
		child_job(p);
	else
		waitpid(-1, 0, 0);
}
