/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_append.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 11:46:03 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/03 17:11:09 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	big_baby_job(t_parser_info *p)
{
	int	array_i;

	array_i = 0;
	while (p->input_files_delimiters[array_i])
	{
		do_in_append(p, array_i, p->pipe_append[array_i]);
		close(p->pipe_append[array_i][0]);
		close(p->pipe_append[array_i][1]);
		array_i++;
	}
	free_everything(p);
	free(p->cmd_path);
	free_double_int(p->pipe_append, (p->pipes_count + 1));
	free_double_char(p->env);
	exit(0);
}

void	make_append_child(t_parser_info *p)
{
	int	status;

	p->child_pids[0] = fork();
	if (!p->child_pids[0])
		big_baby_job(p);
	else
	{
		p->in_append_inprogress = true;
		waitpid(p->child_pids[0], &status, 0);
		if (WIFSIGNALED(status) == false)
			p->in_append_inprogress = false;
	}
}
