/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_append.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 11:46:03 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/22 14:09:34 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	big_baby_job(t_parser_info *p, int **pipe_append)
{
	int	i;
	int	fd;
	int	array_i;

	fd = 0;
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
	int	child;

	child = fork();
	if (!child)
		big_baby_job(p, pipe_append);
	else
		waitpid(child, 0, 0);
}
