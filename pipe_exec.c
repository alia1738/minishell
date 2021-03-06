/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/05 15:14:43 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* --------------------------------- Child --------------------------------- */
static void	pipe_child_process(t_parser_info *p, int pip_i)
{
	signal(SIGINT, SIG_DFL);
	close(p->exit_code_fd[0]);
	close(p->exit_code_fd[1]);
	before_command(p, pip_i);
	if (builtin_check(p, pip_i) < 2)
	{
		builtin_execute(p, pip_i);
		close_remaining_pipes(p, pip_i, p->pipes_count);
		free_close_exit(p);
	}
	if (p->cmd_path)
		execve(p->cmd_path, p->cmd[pip_i], p->env);
	close_remaining_pipes(p, pip_i, p->pipes_count);
	free_close_exit(p);
}

/* --------------------------------- Parent -------------------------------- */
static void	parent_process(t_parser_info *p)
{
	int		i;
	int		status;
	char	temp[3];

	ft_bzero(temp, 3);
	i = -1;
	close_all_pipes(p->pipe_append, (p->pipes_count + 1));
	close_all_pipes(p->pip, (p->pipes_count));
	while (++i < p->pipes_count + 1)
	{
		waitpid(p->child_pids[i], &status, 0);
		p->exit_code = WEXITSTATUS(status);
	}
	close(p->exit_code_fd[1]);
	if (p->signal_in_cmd || p->in_append_inprogress == true)
	{
		read(p->exit_code_fd[0], temp, 3);
		p->exit_code = ft_atoi(temp);
		p->signal_in_cmd = false;
		p->in_append_inprogress = false;
	}
	close(p->exit_code_fd[0]);
	free_double_int(p->pip, p->pipes_count);
	free_double_int(p->pipe_append, (p->pipes_count + 1));
}

/* ----------------------------- Main function ----------------------------- */
void	execute_pipe_execution(t_parser_info *p)
{
	int	i;

	i = -1;
	p->pipe_append = create_pipes(p->pipes_count + 1);
	if (pipe(p->exit_code_fd) < 0)
		exit(1);
	make_append_child(p);
	if (p->in_append_inprogress == true)
	{
		close_all_pipes(p->pipe_append, (p->pipes_count + 1));
		free_double_int(p->pipe_append, (p->pipes_count + 1));
		p->exit_code = 1;
		return ;
	}
	p->pip = create_pipes(p->pipes_count);
	while (++i <= p->pipes_count)
	{
		p->child_pids[i] = fork();
		if (!p->child_pids[i])
			pipe_child_process(p, i);
	}
	parent_process(p);
}
