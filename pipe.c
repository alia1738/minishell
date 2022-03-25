/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/25 12:49:11 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ------------------------------- Child One ------------------------------- */
void	first_child(t_parser_info *p, int **pip, int **pipe_append)
{
	int	in_fd;
	int	out_fd;

	signal(SIGINT, SIG_IGN);
	if (!p->cmd[0][0])
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1);
	}
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	close_pip_append(p, pip, pipe_append, 0);
	in_fd = pipe_final_in_fd(0, p);
	if (in_fd == -1)
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1); // free here
	}
	out_fd = pipe_final_out_fd(0, p);
	if (out_fd == -1)	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1); // free here
	}
	check_in_fd(in_fd, pipe_append[0], pip, 0);
	check_out_fd(out_fd, pip, 0, p->pipes_count);
	if (builtin_execute(p, 0) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
}

/* ------------------------------- Child Two ------------------------------- */
void	middle_child(t_parser_info *p, int **pip, int pip_i, int **pipe_append)
{
	int	in_fd;
	int	out_fd;

	signal(SIGINT, SIG_IGN);
	if (!p->cmd[pip_i][0])
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1);
	}
	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	close_pip_append(p, pip, pipe_append, pip_i);
	in_fd = pipe_final_in_fd(pip_i, p);
	if (in_fd == -1)
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1); // free here
	}
	out_fd = pipe_final_out_fd(pip_i, p);
	if (out_fd == -1)
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1); // free here
	}
	check_in_fd(in_fd, pipe_append[pip_i], pip, pip_i);
	check_out_fd(out_fd, pip, pip_i, p->pipes_count);
	if (builtin_execute(p, pip_i) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
}

/* ------------------------------ Child Three ------------------------------ */
void	last_child(t_parser_info *p, int **pip, int pip_i, int **pipe_append)
{
	int	in_fd;
	int	out_fd;
	
	signal(SIGINT, SIG_IGN);
	if (!p->cmd[pip_i][0])
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(1);
	}
	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	close_pip_append(p, pip, pipe_append, pip_i);
	in_fd = pipe_final_in_fd(pip_i, p);
	if (in_fd == -1)
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(-1); // free here
	}
	out_fd = pipe_final_out_fd(pip_i, p);
	if (out_fd == -1)
	{
		close_all_pipes_fds(p, pip, pipe_append);
		exit(-1); // free here
	}
	check_in_fd(in_fd, pipe_append[pip_i], pip, pip_i);
	check_out_fd(out_fd, pip, pip_i, p->pipes_count);
	if (builtin_execute(p, pip_i) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
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

	pip = create_pipes(p);	
	pipe_append = create_pipe_append(p);
	make_append_child(p, pipe_append);
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
