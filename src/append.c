/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 11:46:03 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/05 14:59:22 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_in_fd(t_parser_info *p, char *input, int pipe_end[2])
{
	char	*temp;
	char	*temp_dollar;

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

int	child_input_append(int array_i, t_parser_info *p, int i, int pipe_end[2])
{
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
			write_in_fd(p, input, pipe_end);
		free(input);
	}
	free(input);
	return (1);
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

void	big_baby_job(t_parser_info *p)
{
	int	array_i;

	array_i = 0;
	close(p->exit_code_fd[0]);
	write(p->exit_code_fd[1], "1", 1);
	while (p->input_files_delimiters[array_i])
	{
		do_in_append(p, array_i, p->pipe_append[array_i]);
		close(p->pipe_append[array_i][0]);
		close(p->pipe_append[array_i][1]);
		array_i++;
	}
	write(p->exit_code_fd[1], "30", 2);
	close(p->exit_code_fd[1]);
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
		if (p->signal_in_cmd == false)
			p->in_append_inprogress = false;
		else
			p->signal_in_cmd = false;
	}
}
