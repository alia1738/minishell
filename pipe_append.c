/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_append.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 11:46:03 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/25 15:25:17 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	big_baby_job(t_parser_info *p, int **pipe_append)
{
	int	i;
	int	array_i;

	signal(SIGINT, SIG_DFL);
	array_i = 0;
	while (p->input_files_delimiters[array_i])
	{
		i = 0;
		while (p->input_files_delimiters[array_i][i])
		{
			if (p->in_arrow_flag[array_i][i] == DOUBLE_ARROW)
				child_input_append(array_i, p, i, pipe_append[array_i]);
			i++;
		}
		close(pipe_append[array_i][0]);
		close(pipe_append[array_i][1]);
		array_i++;
	}
	exit(0);
}

void	make_append_child(t_parser_info *p, int **pipe_append)
{
	int	status;
	p->child_pids[0] = fork();
	if (!p->child_pids[0])
		big_baby_job(p, pipe_append);
	else
	{
		p->in_append_inprogress = true;
		waitpid(p->child_pids[0], &status, 0);
		if (WIFSIGNALED(status) == false)
			p->in_append_inprogress = false;
	}
}
