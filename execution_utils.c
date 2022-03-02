/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 11:46:48 by anasr             #+#    #+#             */
/*   Updated: 2022/03/02 14:15:33 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child_input_append(int array_index, t_parser_info *p, int i, int pipe_end[2])
{
	char	*temp;
	char	*input;

	while (1)
	{
		input = readline("> ");
		if (!ft_strncmp(input, p->input_files_delimiters[array_index][i], \
		ft_strlen(p->input_files_delimiters[array_index][i]) + 1))
			break ;
		if (!p->input_files_delimiters[array_index][i + 1])
		{
			temp = ft_strjoin(input, "\n");
			ft_putstr_fd(temp, pipe_end[1]);
			free(temp);
		}
		free(input);
	}
	free(input);
	return (1);
}

/**********************************************************************
 *************** returns -1 if file cant be accessed ******************
 *************** returns 1 if << std_in is allowed ********************
 *************** returns >1 if it was able to open the file *********** 
**********************************************************************/

int	final_in_fd(int array_index, t_parser_info *p, int pipe_end[2])
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->input_files_delimiters[0][i])
	{
		if (p->in_arrow_flag[array_index][i] == SINGLE_ARROW)
		{
			if (access(p->input_files_delimiters[array_index][i], F_OK) == 1)
				fd = -1;
			if (!p->input_files_delimiters[array_index][i + 1])
				fd = open(p->input_files_delimiters[array_index][i], O_RDONLY, 0640);
		}
		else if (p->in_arrow_flag[array_index][i] == DOUBLE_ARROW)
			fd = child_input_append(array_index, p, i, pipe_end);
		i++;
	}
	return(fd);
}

int	final_out_fd(int array_index, t_parser_info *p)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->output_files[array_index][i])
	{
		if (p->out_arrow_flag[array_index][i] == SINGLE_ARROW)
			fd = open(p->output_files[array_index][i], O_CREAT | O_WRONLY | O_TRUNC, 0640);
		else if (p->out_arrow_flag[array_index][i] == DOUBLE_ARROW)
			fd = open(p->output_files[array_index][i], O_CREAT | O_WRONLY | O_APPEND, 0640);
		if (p->output_files[array_index][i + 1])
			close(fd);
		i++;
	}
	return(fd);
}