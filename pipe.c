/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/10 16:42:43 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_child(t_parser_info *p, int **pip)
{
	int i;

	p->cmd_path[0] = get_cmd_path(p->cmd[0][0]);
	i = 1;
	while (i < p->pipes_count)
	{
		close(pip[i][0]);
		close(pip[i][1]);
		i++;
	}
	close(pip[0][0]);
	dup2(pip[0][1], STDOUT_FILENO);
	close(pip[0][1]); /*****/
	if (p->cmd_path[0])
		execve(p->cmd_path[0], p->cmd[0], 0);
	printf("%sfirst failed :(((((%s\n", RED, RESET);
}

void	middle_child(t_parser_info *p, int **pip, int pip_i)
{
	int	i;

	// printf("%smiddle child pip_i = %d%s\n", GREEN, pip_i, RESET);
	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	i = 0;
	while (i < p->pipes_count)
	{
		if (i != (pip_i - 1) || i != pip_i)
		{
			close(pip[i][0]);
			close(pip[i][1]);
		}
		i++;
	}
	close(pip[pip_i - 1][1]);
	dup2(pip[pip_i - 1][0], STDIN_FILENO);
	close(pip[pip_i - 1][0]); /*****/
	close(pip[pip_i][0]);
	printf("%smiddle failed :(((((%s\n", RED, RESET);
	dup2(pip[pip_i][1], STDOUT_FILENO);
	close(pip[pip_i][1]); /*****/
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
}

void	last_child(t_parser_info *p, int **pip, int pip_i)
{
	int	i;

	// printf("%slast child pip_i = %d%s\n", GREEN, pip_i, RESET);
	p->cmd_path[pip_i] = get_cmd_path(p->cmd[pip_i][0]);
	i = 0;
	while (i < pip_i - 1)
	{
		close(pip[i][0]);
		close(pip[i][1]);
		i++;
	}
	close(pip[pip_i - 1][1]);
	dup2(pip[pip_i - 1][0], STDIN_FILENO);
	close(pip[pip_i - 1][0]);
	if (p->cmd_path[pip_i])
		execve(p->cmd_path[pip_i], p->cmd[pip_i], 0);
	// printf("%slast failed :(((((%s\n", RED, RESET);
}

void	pipe_stuff(t_parser_info *p)
{
	int		i;
	int		**pip;
	int		status;
	pid_t	fork_pid;

	printf("%spipe count: %d%s\n", LIGHT_BLUE, p->pipes_count, RESET);
	pip = ft_calloc((p->pipes_count), sizeof(int *));
	i = 0;
	if (i < p->pipes_count)
	{
		pip[i] = ft_calloc(2, sizeof(int));
		pipe(pip[i]);
		i++;
	}
	// close(pip[1][0]);
	// pipe(pip[0]);
	// pipe(pip[1]);
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
		printf("i = %d\n", i);
		i++;
	}
	// close(pip[0][0]);
	// close(pip[0][1]);
	// close(pip[1][0]);
	// close(pip[1][1]);
	// printf("%spip one closed\n%s", RED, RESET);
	// close(pip[1][0]);
	// close(pip[1][1]);
	// printf("%spip two closed\n%s", RED, RESET);
	while (waitpid(-1, &status, 0) > 0)
	{
		p->exit_code = WEXITSTATUS(status);
		printf("%sI am parent%s\n", BLUE, RESET);
	}
}
