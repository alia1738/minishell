/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 11:46:48 by anasr             #+#    #+#             */
/*   Updated: 2022/03/21 16:56:09 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child_input_append(int array_i, t_parser_info *p, int i, int pipe_end[2])
{
	char	*temp;
	char	*input;

	while (1)
	{
		input = readline("> ");
		if (!ft_strncmp(input, p->input_files_delimiters[array_i][i], \
		ft_strlen(p->input_files_delimiters[array_i][i]) + 1))
			break ;
		if (!p->input_files_delimiters[array_i][i + 1])
		{
			temp = ft_strjoin(expand_dollars_in_str(input, p), "\n");//free expand_dollars_in_str(input)
			ft_putstr_fd(temp, pipe_end[1]);
			free(temp);
		}
		else
			free(input);
	}
	free(input);
	return (1);
}

/* ***************************************************************** *\
|* ************* returns -1 if file cant be accessed *************** *|
|* ************* returns 1 if << std_in is allowed ***************** *|
|* ************* returns >1 if it was able to open the file ******** *|
\* ***************************************************************** */

int	final_in_fd(int array_i, t_parser_info *p, int pipe_end[2])
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->input_files_delimiters[array_i][i])
	{
		if (p->in_arrow_flag[array_i][i] == SINGLE_ARROW)
		{
			if (access(p->input_files_delimiters[array_i][i], F_OK) == 1)
				fd = -1; // perror, free && exit
			if (!p->input_files_delimiters[array_i][i + 1])
				fd = open(p->input_files_delimiters[array_i][i], O_RDONLY, 0640);
		}
		else if (p->in_arrow_flag[array_i][i] == DOUBLE_ARROW)
			fd = child_input_append(array_i, p, i, pipe_end);
		i++;
	}
	return(fd);
}

int	final_out_fd(int array_i, t_parser_info *p)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->output_files[array_i][i])
	{
		if (p->out_arrow_flag[array_i][i] == SINGLE_ARROW)
			fd = open(p->output_files[array_i][i], O_CREAT | O_WRONLY | O_TRUNC, 0640);
		else if (p->out_arrow_flag[array_i][i] == DOUBLE_ARROW)
			fd = open(p->output_files[array_i][i], O_CREAT | O_WRONLY | O_APPEND, 0640);
		if (p->output_files[array_i][i + 1])
			close(fd);
		i++;
	}
	return(fd);
}


int	account_for_in_redirect(int i, int *pipe_append, t_parser_info *p)
{
	int	fd;
	
	fd = final_in_fd(i, p, pipe_append);
	// if (fd == -1) ----------------------------> account for it in final_in_fd
	// 	exit(1);
	if (fd > 1)
		dup2(fd, STDIN_FILENO);
	if (fd == 1) //dup pipe;
	{
		close(pipe_append[1]);
		dup2(pipe_append[0], STDIN_FILENO);
	}
	else if (fd != 1) //close both pipe ends
	{
		close(pipe_append[1]);
		close(pipe_append[0]);
	}
	return(fd);
}

int	account_for_out_redirect(int i, t_parser_info *p)
{
	int	fd;

	fd = final_out_fd(i, p);
	if (fd)
		dup2(fd, STDOUT_FILENO);
	return (fd);
}
