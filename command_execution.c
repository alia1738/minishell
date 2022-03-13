/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Alia <Alia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/12 15:22:52 by Alia             ###   ########.fr       */
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

int	bultin_check(t_parser_info *p)
{
	if (!ft_strncmp(p->cmd[0], "echo", 5))
		;
	else if (!ft_strncmp(p->cmd[0], "cd", 3))
		;
	else if (!ft_strncmp(p->cmd[0], "pwd", 4))
		;
	else if (!ft_strncmp(p->cmd[0], "export", 7))
		;
	else if (!ft_strncmp(p->cmd[0], "unset", 7))
		;
	else if (!ft_strncmp(p->cmd[0], "env", 4))
		;
	return(1);
}

void	execute_command(t_parser_info *p)
{
	pid_t	child;

	if (builtin_check(p)) // builtin return 0 
	{
		child = fork();
		if (!child)
			child_job(p);
		else
			waitpid(-1, 0, 0);
	}
}
