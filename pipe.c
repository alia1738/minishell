/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/01 17:54:47 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* --------------------------------- Child --------------------------------- */
static void	pipe_child_process(t_parser_info *p, int **pip, int pip_i, int **pipe_append)
{
	signal(SIGINT, SIG_DFL);
	before_command(p, pip, pipe_append, pip_i);
	if (builtin_check(p, pip_i) < 2)
	{
		builtin_execute(p, pip_i);
		close_remaining_pipes(pipe_append, pip, pip_i, p->pipes_count);
		free_n_close(p, pip, pipe_append);
		exit(p->exit_code);
	}
	if (p->cmd_path)
		execve(p->cmd_path, p->cmd[pip_i], p->env);
	close_remaining_pipes(pipe_append, pip, pip_i, p->pipes_count);
	free_n_close(p, pip, pipe_append);
	exit(p->exit_code);
}

/* --------------------------------- Parent -------------------------------- */
static void	parent_process(t_parser_info *p, int **pip, int **pipe_append)
{
	int	i;
	int	status;

	i = -1;
	close_all_pipes_fds(p, pip, pipe_append);
	while (++i < p->pipes_count + 1)
	{
		waitpid(p->child_pids[i], &status, 0);
		p->exit_code = WEXITSTATUS(status);
	}
	free_double_int(pip, p->pipes_count);
	free_double_int(pipe_append, (p->pipes_count + 1));
}

/* ----------------------------- Main function ----------------------------- */
void	execute_pipe_execution(t_parser_info *p)
{
	int	i;
	int	**pip;
	int	**pipe_append;

	i = -1;
	pipe_append = create_pipe_append(p);
	make_append_child(p, pipe_append);
	if (p->in_append_inprogress)
	{
		while (++i <= p->pipes_count)
		{
			close(pipe_append[i][0]);
			close(pipe_append[i][1]);
		}
		free_double_int(pipe_append, (p->pipes_count + 1));
		return ;
	}
	pip = create_pipes(p);	
	while (++i <= p->pipes_count)
	{
		p->child_pids[i] = fork();
		if (!p->child_pids[i])
			pipe_child_process(p, pip, i, pipe_append);
	}
	parent_process(p, pip, pipe_append);
}
