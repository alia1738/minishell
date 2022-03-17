/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/17 16:33:45 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* --------------------------------- Child One --------------------------------- */
void	first_child(t_parser_info *p, int **pip)
{
	int i;
	int	in_fd;
	int	out_fd;
	int	pipe_append[2];

	pipe(pipe_append);
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	i = 0;
	while (++i < p->pipes_count)
	{
		close(pip[i][0]);
		close(pip[i][1]);
	}
	in_fd = account_for_in_redirect(0, pipe_append, p);
	out_fd = account_for_out_redirect(0, p);
	check_outpipe_use(out_fd, 0, pip);
	if (builtin_check(p, 0) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
}

/* --------------------------------- Child Two --------------------------------- */
void	middle_child(t_parser_info *p, int **pip, int pip_i)
{
	int	i;
	int	in_fd;
	int	out_fd;
	int	pipe_append[2];

	pipe(pipe_append);
	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	i = -1;
	while (++i < p->pipes_count)
	{
		if (i != (pip_i - 1) && i != pip_i)
		{
			close(pip[i][0]);
			close(pip[i][1]);
		}
	}
	in_fd = account_for_in_redirect(pip_i, pipe_append, p);
	out_fd = account_for_out_redirect(pip_i, p);
	check_inpipe_use(in_fd, pip_i, pip);
	check_outpipe_use(out_fd, pip_i, pip);
	if (builtin_check(p, pip_i) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
}

/* -------------------------------- Child Three -------------------------------- */
void	last_child(t_parser_info *p, int **pip, int pip_i)
{
	int	i;
	int	in_fd;
	int	pipe_append[2];

	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	pipe(pipe_append);
	i = -1;
	while (++i < pip_i - 1)
	{
		close(pip[i][0]);
		close(pip[i][1]);
	}
	in_fd = account_for_in_redirect(pip_i, pipe_append, p);
	check_inpipe_use(in_fd, pip_i, pip);
	account_for_out_redirect(pip_i, p);
	if (builtin_check(p, pip_i) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
}

/* ----------------------------------- Parent ---------------------------------- */
void	parent_process(t_parser_info *p, int **pip)
{
	int		i;
	int		status;

	i = 0;
	while (i < p->pipes_count)
	{
		close(pip[i][0]);
		close(pip[i][1]);
		i++;
	}
	while (waitpid(-1, &status, 0) > 0)
		p->exit_code = WEXITSTATUS(status);
	free_double_int(pip, p->pipes_count);
}

/* ------------------------------- Main function ------------------------------- */
void	pipe_stuff(t_parser_info *p)
{
	int		i;
	int		**pip;
	pid_t	fork_pid;
	int		**pipe_append;
	
	i = 0;
	create_pipes(p, pip);
	create_pipe_append(p, pipe_append);
	while (i <= p->pipes_count)
	{
		fork_pid = fork();
		if (!i && !fork_pid)
			first_child(p, pip);
		else if (i != p->pipes_count && !fork_pid)
			middle_child(p, pip, i);
		else if (!fork_pid)
			last_child(p, pip, i);
		i++;
	}
	parent_process(p, pip);
}
