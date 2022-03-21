/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:25:34 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/21 11:31:10 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pip_append(t_parser_info *p, int **pip, int **append, int pip_i)
{
	int	i;

	i = -1;
	while (++i <= p->pipes_count)
	{
		if (i != p->pipes_count && i != (pip_i - 1) && i != pip_i)
		{
			close(pip[i][0]);
			close(pip[i][1]);
		}
		if (i != pip_i)
		{
			close(append[i][0]);
			close(append[i][1]);
		}
	}
}

void	close_all_pipes_fds(t_parser_info *p, int **pip, int **pipe_append)
{
	int	i;

	i = -1;
	while (++i <= p->pipes_count)
	{
		if (i != p->pipes_count)
		{
			close(pip[i][0]);
			close(pip[i][1]);
		}
		close(pipe_append[i][0]);
		close(pipe_append[i][1]);
		if (p->in_fds[i] > 1)
			close(p->in_fds[i]);
		if (p->out_fds[i] > 1)
			close(p->out_fds[i]);
	}
}
