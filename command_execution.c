/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/25 14:30:35 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	second_child_job(t_parser_info *p, int pipe_append[2], int *in_out)
{
// 	in_fd = final_in_fd(0, p, pipe_append);
// 	out_fd = final_out_fd(0, p);
	account_for_in_redirect(pipe_append, p, in_out[0]);
	if (in_out[1] > 1)
		dup2(in_out[1], STDOUT_FILENO);
	else if (in_out[1] == -1)
		exit(p->exit_code); // free here
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
	//free here
	exit (1);
}

int	builtin_execute(t_parser_info *p, int i)
{
	if (!ft_strncmp(p->cmd[i][0], "echo", 5))
		return (echo(p, p->cmd[i]));
	else if (!ft_strncmp(p->cmd[i][0], "cd", 3))
		return (cd(p, p->cmd[i]));
	else if (!ft_strncmp(p->cmd[i][0], "pwd", 4))
		return (pwd(p));
	else if (!ft_strncmp(p->cmd[i][0], "export", 7))
		return (export(p, p->cmd[i]));
	else if (!ft_strncmp(p->cmd[i][0], "unset", 6))
		return (unset(p, p->cmd[i]));
	else if (!ft_strncmp(p->cmd[i][0], "env", 4))
		return (env(p));
	else if (!ft_strncmp(p->cmd[i][0], "clear", 6))
		return (clear());
	else if (!ft_strncmp(p->cmd[i][0], "exit", 5))
	{
		baby_exit(p, p->cmd[i]);
		return (0);
	}
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
	!ft_strncmp(p->cmd[i][0], "clear", 6) || \
	!ft_strncmp(p->cmd[i][0], "exit", 5))
		return (0);
	return (1);
}

void	execute_command(t_parser_info *p)
{
	int	in_out[2];
	int	pipe_append[2];

	p->child_pids[0] = fork();
	if (!p->child_pids[0])
	{
		signal(SIGINT, SIG_DFL);
		pipe(pipe_append);
		in_out[0] = final_in_fd(0, p, pipe_append);
		in_out[1] = final_out_fd(0, p);
		if (p->cmd[0][0] && builtin_check(p, 0))
			second_child_job(p, pipe_append, in_out);
		exit(0);
	}
	else
	{
		waitpid(p->child_pids[0], 0, 0);
		if (p->cmd[0][0] && !builtin_check(p, 0))
			builtin_execute(p, 0);
	}
}
