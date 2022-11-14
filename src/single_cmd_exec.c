/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/05 16:13:00 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* --------------------------------- Child --------------------------------- */
static void	single_child_process(t_parser_info *p, int pipe_append[2])
{
	if (!p->cmd[0][0] || !builtin_check(p, 0))
		return ;
	account_for_in_redirect(pipe_append, p->in_fd);
	if (p->out_fd > 1)
	{
		dup2(p->out_fd, STDOUT_FILENO);
		close(p->out_fd);
		p->out_fd = 0;
	}
	if (builtin_check(p, 0) == 1)
	{
		builtin_execute(p, 0);
		free_n_exit(p);
	}
	if (p->cmd_path)
		execve(p->cmd_path, p->cmd[0], p->env);
	free_n_exit(p);
}

/* --------------------------------- Parent -------------------------------- */
static void	single_cmd_parent(t_parser_info *p, char temp[3])
{
	int		status;

	if (ft_strrchr(p->cmd[0][0], '/') && \
	!ft_strncmp(ft_strrchr(p->cmd[0][0], '/') + 1, "minishell", 10))
		signal(SIGINT, SIG_IGN);
	waitpid(p->child_pids[0], &status, 0);
	if (ft_strrchr(p->cmd[0][0], '/') && \
	!ft_strncmp(ft_strrchr(p->cmd[0][0], '/') + 1, "minishell", 10))
		signal(SIGINT, handle_signals);
	p->exit_code = WEXITSTATUS(status);
	close(p->exit_code_fd[1]);
	if (p->signal_in_cmd)
	{
		read(p->exit_code_fd[0], temp, 3);
		p->exit_code = ft_atoi(temp);
		p->signal_in_cmd = false;
		close(p->exit_code_fd[0]);
		return ;
	}
	close(p->exit_code_fd[0]);
	if (p->cmd[0][0] && !builtin_check(p, 0))
		builtin_execute(p, 0);
}

/* ----------------------------- Main function ----------------------------- */
void	execute_single_command(t_parser_info *p)
{
	char	temp[3];

	ft_bzero(temp, 3);
	if (pipe(p->exit_code_fd) < 0)
		exit(1);
	p->child_pids[0] = fork();
	if (!p->child_pids[0])
	{
		before_n_after_single_cmd(p, true);
		single_child_process(p, p->pipe_append[0]);
		before_n_after_single_cmd(p, false);
	}
	else
		single_cmd_parent(p, temp);
}
