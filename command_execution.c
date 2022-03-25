/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/25 21:16:37 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_job(t_parser_info *p, int pipe_append[2])
{
	account_for_in_redirect(pipe_append, p->in_fd);
	if (p->out_fd > 1)
		dup2(p->out_fd, STDOUT_FILENO);
	else if (p->in_fd == -1 || p->out_fd == -1)
	{
		free_double_char(p->cmd_path);
		free_everything(p);
		free(pipe_append);
		free(p->env);
		exit(p->exit_code);
	}
	if (p->cmd_absolute_path[0] == true)
		change_cmd(p, 0);
	else
		p->cmd_path[0] = get_cmd_path(p->cmd[0][0], p);
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
	free_double_char(p->cmd_path);
	free_everything(p);
	free(p->env);
	exit (p->exit_code);
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
	int	pipe_append[2];

	p->child_pids[0] = fork();
	if (!p->child_pids[0])
	{
		pipe(pipe_append);
		do_in_append(p, 0, pipe_append);
		p->in_fd = final_in_fd(0, p);
		p->out_fd = final_out_fd(0, p);
		if (p->cmd[0][0] && builtin_check(p, 0))
			child_job(p, pipe_append);
		free(p->env);
		free_everything(p);
		exit(0);
	}
	else
	{
		waitpid(p->child_pids[0], 0, 0);
		if (p->cmd[0][0] && !builtin_check(p, 0))
			builtin_execute(p, 0);
	}
}
