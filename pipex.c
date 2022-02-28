/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 13:33:43 by anasr             #+#    #+#             */
/*   Updated: 2022/02/28 17:05:53 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	first_child(int *out_pipe, t_parser_info *p) //consider only passing what you need to make freeing easier
{
	// if (!p->input_files_delimiters[0])
	// {
		close(out_pipe[0]);
		dup2(out_pipe[1], STDOUT_FILENO);
		execve(p->cmd_path[0], p->cmd[0], NULL);
		perror("ERROR");
		exit(EXIT_FAILURE);
	// }
}

static void	middle_child(int i, int *in_pipe, int *out_pipe, t_parser_info *p) //consider only passing what you need to make freeing easier
{
	// if (!p->input_files_delimiters[i])
	// {
		close(in_pipe[1]);
		dup2(in_pipe[0], STDIN_FILENO);
	// }
	// if (!p->output_files[i])
	// {
		close(out_pipe[0]);
		dup2(out_pipe[1], STDOUT_FILENO);
		execve(p->cmd_path[i], p->cmd[i], NULL);
		perror("ERROR");
		exit(EXIT_FAILURE);
	// }
}

static void	last_child(int	*in_pipe, t_parser_info *p) //consider only passing what you need to make freeing easier
{
	// if (!p->input_files_delimiters[p->pipes_count])
	// {
		close(in_pipe[1]);
		dup2(in_pipe[0], STDIN_FILENO);
		execve(p->cmd_path[p->pipes_count], p->cmd[p->pipes_count], NULL);
		perror("ERROR");
		exit(EXIT_FAILURE);
	// }
}

void	init_pipex(t_parser_info *p)
{
	int	i;
	int	fork_pid;
	int	**pip;

	i = -1;
	pip = ft_calloc(p->pipes_count, sizeof(int *));
	while (++i < p->pipes_count)
	{
		pip[i] = ft_calloc(2, sizeof(int));
		pipe(pip[i]);
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
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	//free int **
}