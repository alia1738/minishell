/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:25:34 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/22 18:48:00 by aalsuwai         ###   ########.fr       */
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
	}
}

int	pipe_final_in_fd(int array_i, t_parser_info *p)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->input_files_delimiters[array_i][i])
	{
		if (p->in_arrow_flag[array_i][i] == SINGLE_ARROW)
		{
			if (access(p->input_files_delimiters[array_i][i], F_OK) == -1)
			{
				// fd = -1; // perror, free && exit
				printf("babyshell: %s: No such file or directory\n", p->input_files_delimiters[array_i][i]);
				p->exit_code = 1;
				return (-1);
			}
			else if (access(p->input_files_delimiters[array_i][i], R_OK) == -1)
			{
				// fd = -1; // perror, free && exit
				printf("babyshell: %s: Permission denied\n",  p->input_files_delimiters[array_i][i]);
				p->exit_code = 1;
				return (-1);
			}
			else if (!p->input_files_delimiters[array_i][i + 1])
				fd = open(p->input_files_delimiters[array_i][i], O_RDONLY, 0640);
		}
		else if (p->in_arrow_flag[array_i][i] == DOUBLE_ARROW)
			fd = 1;
		i++;
	}
	return (fd);
}

int	pipe_final_out_fd(int array_i, t_parser_info *p)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->output_files[array_i][i])
	{
		if (!access(p->output_files[array_i][i], F_OK))
		{
			if (access(p->output_files[array_i][i], W_OK) == -1)
			{
				// perror, free && exit
				printf("babyshell: %s: Permission denied\n", p->output_files[array_i][i]);
				p->exit_code = 1;
				return (-1);
			}
		}
		if (p->out_arrow_flag[array_i][i] == SINGLE_ARROW)
			fd = open(p->output_files[array_i][i], O_CREAT | O_WRONLY | O_TRUNC, 0640);
		else if (p->out_arrow_flag[array_i][i] == DOUBLE_ARROW)
			fd = open(p->output_files[array_i][i], O_CREAT | O_WRONLY | O_APPEND, 0640);
		if (p->output_files[array_i][i + 1])
			close(fd);
		i++;
	}
	return (fd);
}