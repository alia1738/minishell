/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/18 19:10:19 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pip(t_parser_info *p,int **pip, int pip_i)
{
	int i;

	i = -1;
	while (++i < p->pipes_count)
	{
		if (i != (pip_i - 1) && i != pip_i)
		{
			close(pip[i][0]);
			close(pip[i][1]);
		}
	}
}

void	close_append_pipe(t_parser_info *p,int **append, int pip_i)
{
	int i;

	i = -1;
	while (++i <= p->pipes_count)
	{
		if (i != pip_i)
		{
			close(append[i][0]);
			close(append[i][1]);
		}
	}
}

/* --------------------------------- Child One --------------------------------- */
void	first_child(t_parser_info *p, int **pip, int **pipe_append)
{
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	close_pip(p, pip, 0);
	close_append_pipe(p, pipe_append, 0);
	check_in_fd(p->in_fds[0], pipe_append[0], pip, 0);
	check_out_fd(p->out_fds[0], pip, 0, p->pipes_count);
	if (builtin_check(p, 0) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
}

/* --------------------------------- Child Two --------------------------------- */
void	middle_child(t_parser_info *p, int **pip, int pip_i, int **pipe_append)
{
	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	close_pip(p, pip, pip_i);
	close_append_pipe(p, pipe_append, pip_i);
	check_in_fd(p->in_fds[pip_i], pipe_append[pip_i], pip, pip_i);
	check_out_fd(p->out_fds[pip_i], pip, pip_i, p->pipes_count);
	if (builtin_check(p, pip_i) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
}

/* -------------------------------- Child Three -------------------------------- */
void	last_child(t_parser_info *p, int **pip, int pip_i, int **pipe_append)
{
	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	close_pip(p, pip, pip_i);
	close_append_pipe(p, pipe_append, pip_i);
	check_in_fd(p->in_fds[pip_i], pipe_append[pip_i], pip, pip_i);
	check_out_fd(p->out_fds[pip_i], pip, pip_i, p->pipes_count);
	if (builtin_check(p, pip_i) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
}

/* ----------------------------------- Parent ---------------------------------- */
void	parent_process(t_parser_info *p, int **pip, int **pipe_append)
{
	int		i;
	int		status;

	i = -1;
	while (++i < p->pipes_count)
	{
		close(pip[i][0]);
		close(pip[i][1]);
		close(pipe_append[i][0]);
		close(pipe_append[i][1]);
		if (p->in_fds[i] > 1)
			close(p->in_fds[i]);
		if (p->out_fds[i] > 1)
			close(p->out_fds[i]);
	}
	while (waitpid(-1, &status, 0) > 0)
		p->exit_code = WEXITSTATUS(status);
	free_double_int(pip, p->pipes_count);
	free_double_int(pipe_append, (p->pipes_count + 1));
}

/* ------------------------------- Main function ------------------------------- */
void	pipe_stuff(t_parser_info *p)
{
	int		i;
	int		**pip;
	// int		*in_fd;
	// int		*out_fd;
	int		**pipe_append;
	
	// rase everything one level up
	i = 0;
	pip = create_pipes(p);
	pipe_append = create_pipe_append(p);
	p->in_fds = get_in_fds(p, pipe_append);
	p->out_fds = get_out_fds(p);
	while (i <= p->pipes_count)
	{
		p->child_pids[i] = fork();
		if (!i && !p->child_pids[i])
			first_child(p, pip, pipe_append);
		else if (i != p->pipes_count && !p->child_pids[i])
			middle_child(p, pip, i, pipe_append);
		else if (!p->child_pids[i])
			last_child(p, pip, i, pipe_append);
		i++;
	}
	parent_process(p, pip, pipe_append);
}
