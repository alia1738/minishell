/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 18:08:51 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_input_output_files_n_cmds(int array_index, char **specific_cmd, \
t_parser_info *p)
{
	t_sort_indices	index;

	ft_bzero(&index, sizeof(t_sort_indices));
	while (specific_cmd[index.i])
	{
		if ((!ft_strncmp(specific_cmd[index.i], "<<", 2) || \
		!ft_strncmp(specific_cmd[index.i], "<", 1)) && \
		specific_cmd[index.i + 1])
			sort_input(array_index, specific_cmd, &index, p);
		else if ((!ft_strncmp(specific_cmd[index.i], ">>", 2) || \
		!ft_strncmp(specific_cmd[index.i], ">", 1)) && \
		specific_cmd[index.i + 1])
			sort_output(array_index, specific_cmd, &index, p);
		else
			sort_cmds(array_index, specific_cmd, &index, p);
		index.i++;
	}
}

void	save_cmds(char *input, t_parser_info *p)
{
	int		array_index;
	char	*meta[5];

	save_cmds_meta_init(meta);
	p->pipes_count = count_pipes(input);
	allocate_meme_general(p);
	if (p->pipes_count == 0)
	{
		save_cmds_helper(0, input, meta, p);
		p->command_in_action = true;
		execute_single_command(p);
	}
	else
	{
		array_index = 0;
		p->cmd_array = ft_split(input, '|');
		while (array_index < p->pipes_count + 1)
		{
			save_cmds_helper(array_index, p->cmd_array[array_index], meta, p);
			array_index++;
		}
		p->command_in_action = true;
		execute_pipe_execution(p);
	}
	p->command_in_action = false;
}

int	main(int argc, char **argv, char **env)
{
	t_parser_info	p;

	(void)argc;
	(void)argv;
	ft_bzero(&p, sizeof(t_parser_info));
	hide_signal_markers();
	signal(SIGINT, &handle_signals);
	signal(SIGQUIT, SIG_IGN);
	p.env = dup_array(env);
	nested_minishell(&p);
	while (1)
	{
		return_p(&p);
		p.input = readline("\033[1;35mbaby shell\033[2;35m> \e[0m");
		if (!p.input)
		{
			free_double_char(p.env);
			exit(p.exit_code);
		}
		if (p.input[0])
			add_history(p.input);
		if (!check_empty_input(p.input))
			continue ;
		if (check_repeated_meta(p.input, &p) == -1)
			continue ;
		save_cmds(p.input, &p);
		free_everything(&p);
		free(p.cmd_path);
	}
}
