/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/02/18 20:48:13 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_input_append(char *delimiter) // this is shit
{
	char	*input;

	input = 0;
	while (ft_strncmp(input, delimiter, ft_strlen(delimiter)))
	{
		input = readline("> ");
		printf("%s", input);
		free(input);
	}
}

static int	final_in_fd(t_parser_info *p)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->input_files_delimiters[i])
	{
		if (p->in_arrow_flag[i] == SINGLE_ARROW)
		{
			if (access(p->input_files_delimiters[i], F_OK) == 1)
				fd = -1;
			if (!p->input_files_delimiters[i + 1])
				fd = open(p->input_files_delimiters[i], O_RDONLY, 0640);
		}
		else if (p->in_arrow_flag[i] == DOUBLE_ARROW)
			child_input_append(p->input_files_delimiters[i]);
		i++;
	}
	return(fd);
}

static int	final_out_fd(t_parser_info *p)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->output_files[i])
	{
		if (p->out_arrow_flag[i] == SINGLE_ARROW)
			fd = open(p->output_files[i], O_CREAT | O_TRUNC | O_WRONLY, 0640);
		else if (p->out_arrow_flag[i] == DOUBLE_ARROW)
			fd = open(p->output_files[i], O_CREAT | O_APPEND | O_WRONLY, 0640);
		if (p->output_files[i + 1])
			close(fd);
		i++;
	}
	return(fd);
}

static void	child_job(t_parser_info *p)
{
	int	in_fd;
	int	out_fd;

	in_fd = final_in_fd(p);
	out_fd = final_out_fd(p);
	if (in_fd == -1)
		exit(1); // free here
	if (in_fd)
		dup2(in_fd, STDIN_FILENO);
	if (out_fd)
		dup2(out_fd, STDOUT_FILENO);
	execve(p->cmd_path, p->cmd, 0);
	// free here
}

void	execute_command(t_parser_info *p)
{
	pid_t	child;

	child = fork();
	if (!child)
		child_job(p);
	else
		waitpid(-1, 0, 0);
}
