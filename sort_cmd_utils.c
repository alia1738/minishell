/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 16:16:47 by anasr             #+#    #+#             */
/*   Updated: 2022/04/05 13:35:32 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_input(int array_index, char **specific_cmd, \
t_sort_indices *index, t_parser_info *p)
{
	char	*temp;

	if (!ft_strncmp(specific_cmd[index->i], "<<", 2))
		p->in_arrow_flag[array_index][index->in_index] = DOUBLE_ARROW;
	else
		p->in_arrow_flag[array_index][index->in_index] = SINGLE_ARROW;
	if (p->in_arrow_flag[array_index][index->in_index] == SINGLE_ARROW && \
	ft_strchr(specific_cmd[index->i + 1], '$'))
	{
		temp = expand_dollars_in_str(specific_cmd[index->i + 1], p, false);
		if (!temp[0])
		{
			printf("babyshell: %s: ambiguous redirect\n", \
			specific_cmd[index->i + 1]);
			p->in_arrow_flag[array_index][index->in_index] = -1;
		}
		free(specific_cmd[index->i + 1]);
		specific_cmd[index->i + 1] = temp;
	}
	p->input_files_delimiters[array_index][index->in_index++] = \
	specific_cmd[++index->i];
}

void	sort_output(int array_index, char **specific_cmd, \
t_sort_indices *index, t_parser_info *p)
{
	char	*temp;

	if (!ft_strncmp(specific_cmd[index->i], ">>", 2))
		p->out_arrow_flag[array_index][index->out_index] = DOUBLE_ARROW;
	else
		p->out_arrow_flag[array_index][index->out_index] = SINGLE_ARROW;
	if (ft_strchr(specific_cmd[index->i + 1], '$'))
	{
		temp = expand_dollars_in_str(specific_cmd[index->i + 1], p, false);
		if (!temp[0])
		{
			printf("babyshell: %s: ambiguous redirect\n", \
			specific_cmd[index->i + 1]);
			p->out_arrow_flag[array_index][index->out_index] = -1;
		}
		free(specific_cmd[index->i + 1]);
		specific_cmd[index->i + 1] = temp;
	}
	p->output_files[array_index][index->out_index++] = specific_cmd[++index->i];
}

void	sort_cmds(int array_index, char **specific_cmd, \
t_sort_indices *index, t_parser_info *p)
{
	char	*temp;

	if (index->cmd_index == 0 && ft_strchr(specific_cmd[index->i], '/'))
		p->cmd_absolute_path[array_index] = true;
	if (ft_strchr(specific_cmd[index->i], '$'))
	{
		temp = expand_dollars_in_str(specific_cmd[index->i], p, false);
		free(specific_cmd[index->i]);
		specific_cmd[index->i] = temp;
	}
	if (ft_strchr(specific_cmd[index->i], '\'') \
	|| ft_strchr(specific_cmd[index->i], '\"'))
		specific_cmd[index->i] = strcpy_wout_quotes(specific_cmd[index->i]);
	p->cmd[array_index][index->cmd_index++] = specific_cmd[index->i];
}

void	save_cmds_helper(int array_index, char *input, char **meta, \
t_parser_info *p)
{
	allocate_meme_specific(input, array_index, p);
	p->cmd_part[array_index] = ft_split_custom(input, meta, p);
	save_input_output_files_n_cmds(array_index, p->cmd_part[array_index], p);
}

void	save_cmds_meta_init(char **meta)
{
	meta[0] = "<<";
	meta[1] = "<";
	meta[2] = ">>";
	meta[3] = ">";
	meta[4] = 0;
}
