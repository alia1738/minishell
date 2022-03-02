/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 13:33:43 by anasr             #+#    #+#             */
/*   Updated: 2022/03/02 14:23:58 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void account_for_in_redirect(int *in_fd, int array_index, int pipe_append[2], t_parser_info *p)
{
	*in_fd = final_in_fd(0, p, pipe_append);
	if (out_fd)
		dup2(out_fd, STDOUT_FILENO);
}

static void account_for_in_redirect(int *out_fd, int array_index, int pipe_append[2], t_parser_info *p)
{
	*out_fd = final_out_fd(0, p);
	if (in_fd == -1)
		exit(1); //free here
	else if (in_fd == 1) //dup pipe;
	{
		close(pipe_append[1]);
		dup2(pipe_append[0], STDIN_FILENO);
	}
	else if (in_fd)
		dup2(in_fd, STDIN_FILENO);
	if (in_fd != 1) //close both pipe ends
	{
		close(pipe_append[1]);
		close(pipe_append[0]);
	}
}

static void	first_child(int *out_pipe, t_parser_info *p) //consider only passing what you need to make freeing easier
{
	int pipe_append[2];
	int in_fd;
	int out_fd;

	// in_fd = final_in_fd(0, p, pipe_append);
	// out_fd = final_out_fd(0, p);
	// if (in_fd == -1)
	// 	exit(1); //free here
	// else if (in_fd == 1) //dup pipe;
	// {
	// 	close(pipe_append[1]);
	// 	dup2(pipe_append[0], STDIN_FILENO);
	// }
	// else if (in_fd)
	// 	dup2(in_fd, STDIN_FILENO);
	// if (in_fd != 1) //close both pipe ends
	// {
	// 	close(pipe_append[1]);
	// 	close(pipe_append[0]);
	// }
	// if (out_fd)
	// 	dup2(out_fd, STDOUT_FILENO);
	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	printf("FIRST CHILD: %s -- %s\n", p->cmd[0][0], p->cmd_path[0]);
	close(out_pipe[0]);
	dup2(out_pipe[1], STDOUT_FILENO);
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
	close(out_pipe[1]);
		exit(127);
	// }
}

static void	middle_child(int i, int *in_pipe, int *out_pipe, t_parser_info *p) //consider only passing what you need to make freeing easier
{
		p->cmd_path[i] = get_cmd_path(p->cmd[i][0]);
		printf("MIDDLE CHILD: %s -- %s\n", p->cmd[i][0], p->cmd_path[i]);
	// if (!p->input_files_delimiters[i])
	// {
		close(in_pipe[1]);
		dup2(in_pipe[0], STDIN_FILENO);
	// }
	// if (!p->output_files[i])
	// {
		close(out_pipe[0]);
		dup2(out_pipe[1], STDOUT_FILENO);
		if (p->cmd_path[i])
			execve(p->cmd_path[i], p->cmd[i], 0);
		exit(127);
	// }
}

static void	last_child(int	*in_pipe, t_parser_info *p) //consider only passing what you need to make freeing easier
{
	// if (!p->input_files_delimiters[p->pipes_count])
	// {
		close(in_pipe[1]);
		dup2(in_pipe[0], STDIN_FILENO);
		p->cmd_path[p->pipes_count] = get_cmd_path(p->cmd[p->pipes_count][0]);
		printf("LAST CHILD: %s -- %s\n", p->cmd[p->pipes_count][0], p->cmd_path[p->pipes_count]);
		if (p->cmd_path[p->pipes_count])
			execve(p->cmd_path[p->pipes_count], p->cmd[p->pipes_count], 0);
		close(in_pipe[0]);
		exit(127);
	// }
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
		printf("PIPE: %d\n", i);
	}
	i = -1;
	while (++i < p->pipes_count + 1)
	{
		fork_pid = fork();//protect in case of failure
		if (fork_pid == 0 &&  i == 0)
			first_child(pip[0], p);
		else if (fork_pid == 0 &&  i == p->pipes_count)
			last_child(pip[i - 1], p);//check the "i - 1"
		else if (fork_pid == 0)
			middle_child(i, pip[i - 1], pip[i], p);
		printf("WEEE: %d\n", i);
	}
	while (waitpid(-1, &status, 0) > 0)
	{
		p->exit_code = WEXITSTATUS(status);
		printf("waiting\n");
	}
	// sleep(5);
	// printf("waiting\n");

	//free int **
}