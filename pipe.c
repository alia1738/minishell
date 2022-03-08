/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 16:42:32 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/08 17:21:51 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_child(t_parser_info *p, int *out_pipe)
{
	;
}

void	first_middle(t_parser_info *p, int *in_pipe, int *out_pipe)
{
	;
}

void	last_child(t_parser_info *p, int *in_pipe)
{
	;
}

void	pipe_stuff(t_parser_info *p)
{
	int		i;
	int		**pip;
	int		status;
	pid_t	fork_pid;

	pip = ft_calloc(p->pipes_count, sizeof(int *));
	i = 0;
	while (i <= p->pipes_count)
	{
		if (i != p->pipes_count)
		{
			pip[i] = ft_calloc(2, sizeof(int));
			pipe(pip[i]);
		}
		fork_pid = fork();
		if (!i && !fork_pid)
			first_child(p, pip[i]);
		else if (i != p->pipes_count && !fork_pid)
			middle_child(p, pip[i - 1], pip[i]);
		else if (!fork_pid)
			last_child(p, pip[i - 1]);
		if (i == p->pipes_count && fork_pid)
		{
			while (waitpid(-1, &status, 0) > 0)
			{
				p->exit_code = WEXITSTATUS(status);
				printf("%sI am parent%s\n", BLUE, RESET);
			}
		}
		i++;
	}
}