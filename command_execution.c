/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Alia <Alia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/19 15:02:27 by Alia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_job(t_parser_info *p, int pipe_append[2])
{
	account_for_in_redirect(pipe_append, p);
	if (p->out_fds[0])
		dup2(p->out_fds[0], STDOUT_FILENO);
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
	//free here
	if (p->in_fds[0] == 1)
		close(pipe_append[0]);
	else if (p->in_fds[0])
		close(p->in_fds[0]);
	if (p->out_fds[0])
		close(p->out_fds[0]);
	exit(127);
}

int	builtin_execute(t_parser_info *p, int i)
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
	else if (!ft_strncmp(p->cmd[i][0], "clear", 6))
		return (clear());
	return (-1);
}

int	builtin_check(t_parser_info *p, int i)
{
	if (!ft_strncmp(p->cmd[i][0], "echo", 5) || \
	!ft_strncmp(p->cmd[i][0], "cd", 3) || \
	!ft_strncmp(p->cmd[i][0], "pwd", 4) || \
	!ft_strncmp(p->cmd[i][0], "export", 7) || \
	!ft_strncmp(p->cmd[i][0], "unset", 6) || \
	!ft_strncmp(p->cmd[i][0], "env", 4) || \
	!ft_strncmp(p->cmd[i][0], "clear", 6))
		return (0);
	return (1);
}

void	execute_command(t_parser_info *p)
{
	pid_t	child;
	int		pipe_append[2];

	// check for fd; // fork && check for bultin if it there exit and execute it by parent .. if not do the command execution here
	p->child_pids[0] = fork;
	if (!p->child_pids[0])
	{
		pipe(pipe_append);
		p->in_fds[0] = final_in_fd(0, p, pipe_append);
		p->out_fds[0] = final_out_fd(0, p);
		if (builtin_check(p, 0))
		{
			p->child_pids[1] = fork();
			if (!p->child_pids[1])
				child_job(p, pipe_append); // get the open fds
			else
			{
				waitpid(-1, 0, 0);
				exit(p->exit_code);
			}
		}
		else
			exit(p->exit_code);
	}
	else
	{
		waitid(p->child_pids[0], 0, 0);
		if (!builtin_check(p, 0))
			builtin_execute(p, 0)
	}
	// if (builtin_execute(p, 0) == -1) // if builtin return 0 
	// {
	// 	child = fork();
	// 	if (!child)
	// 		child_job(p); // get the open fds
	// 	else
	// 		waitpid(-1, 0, 0);
	// }
}
