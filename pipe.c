/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/21 13:42:12 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ------------------------------- Child One ------------------------------- */
void	first_child(t_parser_info *p, int **pip, int **pipe_append)
{
	if (!p->cmd[0][0])
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1);
	}
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	close_pip_append(p, pip, pipe_append, 0);
	check_in_fd(p->in_fds[0], pipe_append[0], pip, 0);
	check_out_fd(p->out_fds[0], pip, 0, p->pipes_count);
	if (builtin_execute(p, 0) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
}

/* ------------------------------- Child Two ------------------------------- */
void	middle_child(t_parser_info *p, int **pip, int pip_i, int **pipe_append)
{
	if (!p->cmd[pip_i][0])
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1);
	}
	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	close_pip_append(p, pip, pipe_append, pip_i);
	check_in_fd(p->in_fds[pip_i], pipe_append[pip_i], pip, pip_i);
	check_out_fd(p->out_fds[pip_i], pip, pip_i, p->pipes_count);
	if (builtin_execute(p, pip_i) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
}

/* ------------------------------ Child Three ------------------------------ */
void	last_child(t_parser_info *p, int **pip, int pip_i, int **pipe_append)
{
	if (!p->cmd[pip_i][0])
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1);
	}
	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	close_pip_append(p, pip, pipe_append, pip_i);
	check_in_fd(p->in_fds[pip_i], pipe_append[pip_i], pip, pip_i);
	check_out_fd(p->out_fds[pip_i], pip, pip_i, p->pipes_count);
	if (builtin_execute(p, pip_i) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
}

/* --------------------------------- Parent -------------------------------- */

void	main_child_process(t_parser_info *p, int **pip, int **pipe_append)
{
	int	status;

	close_all_pipes_fds(p, pip, pipe_append);
	while (waitpid(-1, &status, 0) > 0)
		p->exit_code = WEXITSTATUS(status);
	free_double_int(pip, p->pipes_count);
	free_double_int(pipe_append, (p->pipes_count + 1));
	exit(p->exit_code);
}

/* ----------------------------- Main function ----------------------------- */
void	pipe_stuff(t_parser_info *p)
{
	int	i;
	int	**pip;
	int	**pipe_append;

	p->child_pids[0] = fork();
	if (!p->child_pids[0])
	{
		pip = create_pipes(p);
		pipe_append = create_pipe_append(p);
		get_in_out_fds(p, pipe_append);
		i = 0;
		while (++i <= p->pipes_count + 1)
		{
			p->child_pids[i] = fork();
			if (!i && !p->child_pids[i])
				first_child(p, pip, pipe_append);
			else if (i != p->pipes_count && !p->child_pids[i])
				middle_child(p, pip, (i - 1), pipe_append);
			else if (!p->child_pids[i])
				last_child(p, pip, (i - 1), pipe_append);
		}
		main_child_process(p, pip, pipe_append);
	}
	else
		waitpid(p->child_pids[0], 0, 0);
}
