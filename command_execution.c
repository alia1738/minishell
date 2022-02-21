/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Alia <Alia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/02/21 18:57:48 by Alia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	child_input_append(t_parser_info *p, int i, int pipe_end[2])
{
	char	*temp;
	char	*input;

	while (1)
	{
		input = readline("> ");
		if (!ft_strncmp(input, p->input_files_delimiters[i], \
		ft_strlen(p->input_files_delimiters[i]) + 1))
			break ;
		if (!p->input_files_delimiters[i + 1])
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

static int	final_in_fd(t_parser_info *p, int pipe_end[2])
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
			fd = child_input_append(p, i, pipe_end);
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
			fd = open(p->output_files[i], O_CREAT | O_WRONLY | O_TRUNC, 0640);
		else if (p->out_arrow_flag[i] == DOUBLE_ARROW)
			fd = open(p->output_files[i], O_CREAT | O_WRONLY | O_APPEND, 0640);
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
	int	pipe_end[2];

	pipe(pipe_end);
	in_fd = final_in_fd(p, pipe_end);
	out_fd = final_out_fd(p);
	if (in_fd == -1)
		exit(1); // free here
	else if (in_fd == 1) // dup pipe;
	{
		close(pipe_end[1]);
		dup2(pipe_end[0], STDIN_FILENO);
	}
	else if (in_fd)
		dup2(in_fd, STDIN_FILENO);
	if (in_fd != 1) //close both pipe ends
	{
		close(pipe_end[1]);
		close(pipe_end[0]);
	}
	if (out_fd)
		dup2(out_fd, STDOUT_FILENO);
	execve(p->cmd_path, p->cmd, 0);
	// free here
	exit(1);
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
