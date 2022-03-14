/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/14 12:46:47 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_job(t_parser_info *p)
{
	int	in_fd;
	int	out_fd;
	int	pipe_append[2];

	pipe(pipe_append);
	// in_fd = final_in_fd(0, p, pipe_append);
	// out_fd = final_out_fd(0, p);
	in_fd = account_for_in_redirect(0, pipe_append, p);
	out_fd = account_for_out_redirect(0, p);
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

int	builtin_check(t_parser_info *p, int i)
{
	if (!ft_strncmp(p->cmd[i][0], "echo", 5))
		return (echo(p->cmd[i]));
	else if (!ft_strncmp(p->cmd[i][0], "cd", 3))
		return (cd(p->cmd[i]));
	else if (!ft_strncmp(p->cmd[i][0], "pwd", 4))
		return (pwd());
	else if (!ft_strncmp(p->cmd[i][0], "export", 7))
	{
		export(p, p->cmd[i]);
		return (0);
	}
	else if (!ft_strncmp(p->cmd[i][0], "unset", 6))
		;
	else if (!ft_strncmp(p->cmd[i][0], "env", 4))
		return (env(p));
	else if (!ft_strncmp(p->cmd[i][0], "exit", 5))
		;
		// return (baby_exit());
	return (1);
}

void	execute_command(t_parser_info *p)
{
	pid_t	child;

	if (builtin_check(p, 0) == 1) // builtin return 0 
	{
		child = fork();
		if (!child)
			child_job(p);
		else
			waitpid(-1, 0, 0);
	}
}
