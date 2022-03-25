/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_append.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 11:46:03 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/25 18:32:45 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	big_baby_job(t_parser_info *p, int **pipe_append)
{
	int	array_i;

	array_i = 0;
	while (p->input_files_delimiters[array_i])
	{
		do_in_append(p, array_i, pipe_append[array_i]);
		close(pipe_append[array_i][0]);
		close(pipe_append[array_i][1]);
		array_i++;
	}
	free_everything(p);
	free_double_int(pipe_append, (p->pipes_count + 1));
	free(p->env);
	exit(0);
}

void	make_append_child(t_parser_info *p, int **pipe_append)
{
	p->child_pids[0] = fork();
	if (!p->child_pids[0])
		big_baby_job(p, pipe_append);
	else
		waitpid(p->child_pids[0], 0, 0);
}
