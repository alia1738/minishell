/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 13:33:43 by anasr             #+#    #+#             */
/*   Updated: 2022/03/04 18:41:33 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	first_child(int *out_pipe, t_parser_info *p) //consider only passing what you need to make freeing easier
{
	// int pipe_append[2];
	// int in_fd;
	// int out_fd;

	// in_fd = account_for_in_redirect(0, pipe_append, 0, p);
	// out_fd = account_for_out_redirect(0, out_pipe, p);
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	close(out_pipe[0]);
	dup2(out_pipe[1], STDOUT_FILENO);
	// printf("FIRST CHILD: %s -- %s\n", p->cmd[0][0], p->cmd_path[0]);
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
	close(out_pipe[1]);
		exit(127);
}

static void	middle_child(int i, int *in_pipe, int *out_pipe, t_parser_info *p) //consider only passing what you need to make freeing easier
{
	// int pipe_append[2];
	// int in_fd;
	// int out_fd;

	// in_fd = account_for_in_redirect(i, pipe_append, in_pipe, p);
	// out_fd = account_for_out_redirect(i, out_pipe, p);
	p->cmd_path[i] = get_cmd_path(p->cmd[i][0]);
	// // printf("MIDDLE CHILD: %s -- %s\n", p->cmd[i][0], p->cmd_path[i]);
	close(in_pipe[1]);
	dup2(in_pipe[0], STDIN_FILENO);
	close(out_pipe[0]);
	dup2(out_pipe[1], STDOUT_FILENO);
	if (p->cmd_path[i])
		execve(p->cmd_path[i], p->cmd[i], 0);
	exit(127);
}

static void	last_child(int i, int	*in_pipe, t_parser_info *p) //consider only passing what you need to make freeing easier
{
	// int pipe_append[2];
	// int in_fd;
	// int out_fd;

	(void)i;
	// in_fd = account_for_in_redirect(i, pipe_append, in_pipe, p);
	// out_fd = account_for_out_redirect(i, 0, p);
	p->cmd_path[p->pipes_count] = get_cmd_path(p->cmd[p->pipes_count][0]);
	close(in_pipe[1]);
	dup2(in_pipe[0], STDIN_FILENO);
	// printf("LAST CHILD: %s -- %s\n", p->cmd[p->pipes_count][0], p->cmd_path[p->pipes_count]);
	if (p->cmd_path[p->pipes_count])
		execve(p->cmd_path[p->pipes_count], p->cmd[p->pipes_count], 0);
	close(in_pipe[0]);
	exit(127);
}

void	init_pipex(t_parser_info *p)
{
	int	i;
	int	fork_pid;
	int	status;
	int	**pip;

	i = -1;
	pip = ft_calloc(p->pipes_count, sizeof(int *));
	while (++i < p->pipes_count)
	{
		pip[i] = ft_calloc(2, sizeof(int));
		pipe(pip[i]);
		// printf("PIPE: %d\n", i);
	}
	i = -1;
	fork_pid = fork();
	if (!fork_pid)
	{
		while (++i < p->pipes_count + 1)
		{
			fork_pid = fork(); //protect in case of failure
			if (!fork_pid && !i)
				first_child(pip[0], p);
			else if (!fork_pid && i != p->pipes_count)
				middle_child(i, pip[i - 1], pip[i], p);
			else if (!fork_pid && i == p->pipes_count)
				last_child(i, pip[i - 1], p); //check the "i - 1"
			if (i == p->pipes_count && fork_pid)
			{
				waitpid(-1, &status, 0);
				exit(1);
			}
			// printf("WEEE: %d\n", i);
		}
	}
	else
	{
		waitpid(-1, &status, 0);
		printf("done\n");
	}
	// while (waitpid(-1, &status, 0) > 0)
	// {
	// 	p->exit_code = WEXITSTATUS(status);
	// 	printf("waiting\n");
	// }
	//free int **
}
