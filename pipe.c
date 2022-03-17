/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/17 13:09:29 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_inpipe_use(int flag, int index, int **pip)
{
	if (!flag)
	{
		close(pip[index - 1][1]);
		dup2(pip[index - 1][0], STDIN_FILENO);
		close(pip[index - 1][0]);
	}
	else if (flag)
	{
		close(pip[index - 1][0]);
		close(pip[index - 1][1]);
	}
}

void	check_outpipe_use(int flag, int index, int **pip)
{
	if (!flag)
	{
		close(pip[index][0]);
		dup2(pip[index][1], STDOUT_FILENO);
		close(pip[index][1]);
	}
	else if (flag)
	{
		close(pip[index][0]);
		close(pip[index][1]);
	}
}


/* --------------------------------- Child One --------------------------------- */
void	first_child(t_parser_info *p, int **pip)
{
	int i;
	int	pipe_append[2];
	int	out_fd;

	pipe(pipe_append);
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	i = 1;
	while (i < p->pipes_count)
	{
		close(pip[i][0]);
		close(pip[i][1]);
		i++;
	}
	account_for_in_redirect(0, pipe_append, p);
	out_fd = account_for_out_redirect(0, p);
	if (!out_fd)
		check_outpipe_use(0, 0, pip);
	else
		check_outpipe_use(1, 0, pip);
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
	i = 0;
	while (i < p->pipes_count)
	{
		if (i != (pip_i - 1) && i != pip_i)
		{
			close(pip[i][0]);
			close(pip[i][1]);
		}
		i++;
	}
	in_fd = account_for_in_redirect(pip_i, pipe_append, p);
	out_fd = account_for_out_redirect(pip_i, p);
	if (!in_fd)
		check_inpipe_use(0, pip_i, pip);
	else
		check_inpipe_use(1, pip_i, pip);
	if (!out_fd)
		check_outpipe_use(0, pip_i, pip);
	else
		check_outpipe_use(1, pip_i, pip);
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
	i = 0;
	while (i < pip_i - 1)
	{
		close(pip[i][0]);
		close(pip[i][1]);
		i++;
	}
	in_fd = account_for_in_redirect(pip_i, pipe_append, p);
	if (!in_fd)
		check_inpipe_use(0, pip_i, pip);
	else
		check_inpipe_use(1, pip_i, pip);
	account_for_out_redirect(pip_i, p);
	if (builtin_check(p, pip_i) != -1)
		exit(p->exit_code); //free and exit with correct code
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
}

/* ------------------------------- Main function ------------------------------- */
void	pipe_stuff(t_parser_info *p)
{
	int		i;
	int		**pip;
	int		status;
	pid_t	fork_pid;

	pip = ft_calloc((p->pipes_count), sizeof(int *));
	i = 0;
	while (i < p->pipes_count)
	{
		pip[i] = ft_calloc(2, sizeof(int));
		pipe(pip[i]);
		i++;
	}
	i = 0;
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
