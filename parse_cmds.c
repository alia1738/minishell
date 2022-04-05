/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 12:16:16 by anasr             #+#    #+#             */
/*   Updated: 2022/04/05 12:17:20 by anasr            ###   ########.fr       */
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
