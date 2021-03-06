/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 12:05:27 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/04 12:45:19 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	in_fds_errors(t_parser_info *p, int array_i, int i)
{
	if (access(p->input_files_delimiters[array_i][i], F_OK) == -1)
	{
		printf("babyshell: %s: No such file or directory\n", \
		p->input_files_delimiters[array_i][i]);
		p->exit_code = 1;
		return (-1);
	}
	else if (access(p->input_files_delimiters[array_i][i], R_OK) == -1)
	{
		printf("babyshell: %s: Permission denied\n", \
		p->input_files_delimiters[array_i][i]);
		p->exit_code = 1;
		return (-1);
	}
	return (0);
}

static int	out_fds_errors(t_parser_info *p, int array_i, int i)
{
	if (p->out_arrow_flag[array_i][i] == -1)
	{
		p->exit_code = 1;
		return (-1);
	}
	if (!access(p->output_files[array_i][i], F_OK))
	{
		if (access(p->output_files[array_i][i], W_OK) == -1)
		{
			printf("babyshell: %s: Permission denied\n", \
			p->output_files[array_i][i]);
			p->exit_code = 1;
			return (-1);
		}
	}
	return (0);
}

int	final_in_fd(int array_i, t_parser_info *p)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->input_files_delimiters[array_i][i])
	{
		if (p->in_arrow_flag[array_i][i] == -1)
		{
			p->exit_code = 1;
			return (-1);
		}
		if (p->in_arrow_flag[array_i][i] == SINGLE_ARROW)
		{
			if (in_fds_errors(p, array_i, i) == -1)
				return (-1);
			else if (!p->input_files_delimiters[array_i][i + 1])
				fd = open(p->input_files_delimiters[array_i][i], \
				O_RDONLY, 0640);
		}
		else if (p->in_arrow_flag[array_i][i] == DOUBLE_ARROW)
			fd = 1;
		i++;
	}
	return (fd);
}

int	final_out_fd(int array_i, t_parser_info *p)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->output_files[array_i][i])
	{
		if (out_fds_errors(p, array_i, i) == -1)
			return (-1);
		if (p->out_arrow_flag[array_i][i] == SINGLE_ARROW)
			fd = open(p->output_files[array_i][i], O_CREAT | \
			O_WRONLY | O_TRUNC, 0640);
		else if (p->out_arrow_flag[array_i][i] == DOUBLE_ARROW)
			fd = open(p->output_files[array_i][i], O_CREAT | \
			O_WRONLY | O_APPEND, 0640);
		if (p->output_files[array_i][i + 1])
			close(fd);
		i++;
	}
	return (fd);
}

/* ***************************************************************** *\
|* ************* returns -1 if file cant be accessed *************** *|
|* ************* returns 1 if << std_in is allowed ***************** *|
|* ************* returns >1 if it was able to open the file ******** *|
\* ***************************************************************** */
