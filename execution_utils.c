/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 11:46:48 by anasr             #+#    #+#             */
/*   Updated: 2022/04/03 17:28:08 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child_input_append(int array_i, t_parser_info *p, int i, int pipe_end[2])
{
	char	*temp;
	char	*temp_dollar;
	char	*input;

	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (ft_smartncmp(input, p->input_files_delimiters[array_i][i], \
		ft_strlen(p->input_files_delimiters[array_i][i])))
			break ;
		if (!p->input_files_delimiters[array_i][i + 1])
		{
			temp = ft_strjoin(input, "\n");
			if (ft_strchr(temp, '$'))
			{
				temp_dollar = expand_dollars_in_str(temp, p, true);
				free(temp);
				temp = temp_dollar;
			}
			ft_putstr_fd(temp, pipe_end[1]);
			free(temp);
		}
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
		close_all_pipes_fds(p);
		free_double_int(p->pip, p->pipes_count);
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

void	do_in_append(t_parser_info *p, int array_i, int pipe_end[2])
{
	int	i;

	i = 0;
	signal(SIGINT, append_handler);
	while (p->input_files_delimiters[array_i][i])
	{
		if (p->in_arrow_flag[array_i][i] == DOUBLE_ARROW)
			child_input_append(array_i, p, i, pipe_end);
		i++;
	}
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
			// printf("babyshell: %s: ambiguous redirect\n", p->input_files_delimiters[array_i][i]);
			p->exit_code = 1;
			return (-1);
		}
		if (p->in_arrow_flag[array_i][i] == SINGLE_ARROW)
		{
			if (access(p->input_files_delimiters[array_i][i], F_OK) == -1)
			{
				printf("babyshell: %s: No such file or directory\n", p->input_files_delimiters[array_i][i]);
				p->exit_code = 1;
				return (-1);
			}
			else if (access(p->input_files_delimiters[array_i][i], R_OK) == -1)
			{
				printf("babyshell: %s: Permission denied\n", p->input_files_delimiters[array_i][i]);
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

int	final_out_fd(int array_i, t_parser_info *p)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (p->output_files[array_i][i])
	{
		if (p->out_arrow_flag[array_i][i] == -1)
		{
			// printf("babyshell: %s: ambiguous redirect\n", p->output_files[array_i][i]);
			p->exit_code = 1;
			return (-1);
		}
		if (!access(p->output_files[array_i][i], F_OK))
		{
			if (access(p->output_files[array_i][i], W_OK) == -1)
			{
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

void	account_for_in_redirect(int *pipe_append, int in_fd)
{	
	if (in_fd > 1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
		in_fd = 0;
	}
	if (in_fd == 1)
	{
		close(pipe_append[1]);
		dup2(pipe_append[0], STDIN_FILENO);
		close(pipe_append[0]);
	}
	else if (in_fd != 1)
	{
		close(pipe_append[1]);
		close(pipe_append[0]);
	}
}
