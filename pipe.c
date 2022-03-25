/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/25 20:53:47 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ------------------------------- Child One ------------------------------- */
void	first_child(t_parser_info *p, int **pip, int **pipe_append)
{
	before_command(p, pip, pipe_append, 0);
	if (!builtin_check(p, 0))
	{
		builtin_execute(p, 0);
		close_remaining_pipes(pipe_append, pip, 0, p->pipes_count);
		free_n_close(p, pip, pipe_append);
		exit(p->exit_code);
	}
	if (p->cmd_absolute_path[0] == true)
		change_cmd(p, 0);
	else
		p->cmd_path[0] = get_cmd_path(p->cmd[0][0], p);
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
	close_remaining_pipes(pipe_append, pip, 0, p->pipes_count);
	free_n_close(p, pip, pipe_append);
	exit(p->exit_code);
}

/* ------------------------------- Child Two ------------------------------- */
void	middle_child(t_parser_info *p, int **pip, int pip_i, int **pipe_append)
{
	before_command(p, pip, pipe_append, pip_i);
	if (!builtin_check(p, pip_i))
	{
		builtin_execute(p, pip_i);
		close_remaining_pipes(pipe_append, pip, pip_i, p->pipes_count);
		free_n_close(p, pip, pipe_append);
		exit(p->exit_code);
	}
	if (p->cmd_absolute_path[pip_i] == true)
		change_cmd(p, pip_i);
	else
		p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0], p);
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
	close_remaining_pipes(pipe_append, pip, pip_i, p->pipes_count);
	free_n_close(p, pip, pipe_append);
	exit(p->exit_code);
}

/* ------------------------------ Child Three ------------------------------ */
void	last_child(t_parser_info *p, int **pip, int pip_i, int **pipe_append)
{
	before_command(p, pip, pipe_append, pip_i);
	if (!builtin_check(p, pip_i))
	{
		builtin_execute(p, pip_i);
		close_remaining_pipes(pipe_append, pip, pip_i, p->pipes_count);
		free_n_close(p, pip, pipe_append);
		exit(p->exit_code);
	}
	if (p->cmd_absolute_path[pip_i] == true)
		change_cmd(p, pip_i);
	else
		p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0], p);
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
	close_remaining_pipes(pipe_append, pip, pip_i, p->pipes_count);
	free_n_close(p, pip, pipe_append);
	exit(p->exit_code);
}

/* --------------------------------- Parent -------------------------------- */

void	parent_process(t_parser_info *p, int **pip, int **pipe_append)
{
	int	status;

	close_all_pipes_fds(p, pip, pipe_append);
	while (waitpid(-1, &status, 0) > 0)
		p->exit_code = WEXITSTATUS(status);
	free_double_int(pip, p->pipes_count);
	free_double_int(pipe_append, (p->pipes_count + 1));
}

/* ----------------------------- Main function ----------------------------- */
void	pipe_stuff(t_parser_info *p)
{
	int	i;
	int	**pip;
	int	**pipe_append;

	pipe_append = create_pipe_append(p);
	make_append_child(p, pipe_append);
	pip = create_pipes(p);	
	i = -1;
	while (++i <= p->pipes_count)
	{
		p->child_pids[i] = fork();
		if (!i && !p->child_pids[i])
			first_child(p, pip, pipe_append);
		else if (i != p->pipes_count && !p->child_pids[i])
			middle_child(p, pip, i, pipe_append);
		else if (!p->child_pids[i])
			last_child(p, pip, i, pipe_append);
	}
	parent_process(p, pip, pipe_append);
}
