/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 11:46:48 by anasr             #+#    #+#             */
/*   Updated: 2022/04/05 12:19:47 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sig_close_n_free(t_parser_info *p)
{
	if (!p->pipes_count)
	{
		close(p->pipe_append[0][0]);
		close(p->pipe_append[0][1]);
		close(p->exit_code_fd[1]);
		free_double_int(p->pipe_append, 1);
	}
	else
	{
		close(p->exit_code_fd[1]);
		close_all_pipes(p->pipe_append, (p->pipes_count + 1));
		free_double_int(p->pipe_append, (p->pipes_count + 1));
	}
}

void	append_handler(int signum)
{
	t_parser_info	*p;

	p = return_p(NULL);
	if (signum == SIGINT)
	{
		sig_close_n_free(p);
		free_everything(p);
		free_double_char(p->env);
		exit(0);
	}
}
