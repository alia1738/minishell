/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 02:46:33 by anasr             #+#    #+#             */
/*   Updated: 2022/04/05 13:41:24 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_everything(t_parser_info *p)
{
	free_double_int(p->in_arrow_flag, p->pipes_count + 1);
	free_double_int(p->out_arrow_flag, p->pipes_count + 1);
	free_triple_char(p->cmd_part);
	if (p->pipes_count > 0)
		free_double_char(p->cmd_array);
	free_triple_char_partial(p->cmd, p);
	free_triple_char_partial(p->output_files, p);
	free_triple_char_partial(p->input_files_delimiters, p);
	free(p->child_pids);
	free(p->cmd_absolute_path);
	free(p->input);
}

void	allocate_meme_general(t_parser_info *p)
{
	p->cmd_part = (char ***)ft_calloc_p(p->pipes_count + 2, sizeof(char **));
	p->cmd = (char ***)ft_calloc_p(p->pipes_count + 2, sizeof(char **));
	p->in_arrow_flag = (int **)ft_calloc_p(p->pipes_count + 1, sizeof(int *));
	p->out_arrow_flag = (int **)ft_calloc_p(p->pipes_count + 1, sizeof(int *));
	p->output_files = (char ***)ft_calloc_p(p->pipes_count + 2, \
	sizeof(char **));
	p->input_files_delimiters = (char ***)ft_calloc_p(p->pipes_count + 2, \
	sizeof(char **));
	p->child_pids = (int *)ft_calloc_p(p->pipes_count + 1, sizeof(int));
	p->cmd_absolute_path = (bool *)ft_calloc_p(p->pipes_count + 1, \
	sizeof(bool));
}

void	allocate_meme_specific(char *str, int array_index, t_parser_info *p)
{
	p->cmd[array_index] = (char **)ft_calloc_p \
	(count_cmds_wout_meta(str, p) + 1, sizeof(char *));
	if (count_in_redirections(str) > 0)
		p->in_arrow_flag[array_index] = (int *)ft_calloc_p \
		(count_in_redirections(str), sizeof(int));
	if (count_out_redirections(str) > 0)
		p->out_arrow_flag[array_index] = (int *)ft_calloc_p \
		(count_out_redirections(str), sizeof(int));
	p->output_files[array_index] = (char **)ft_calloc_p \
	(count_out_redirections(str) + 1, sizeof(char *));
	p->input_files_delimiters[array_index] = (char **)ft_calloc_p \
	(count_in_redirections(str) + 1, sizeof(char *));
}

/*

cmd	-- char ***
	-- allocated partially
	-- (p->pipes_count + 2) char**
	-- allocated count_cmds_wout_meta() char*'s for each char**

cmd_part	-- char ***
			-- allocated fully     
			-- (p->pipes_count + 2) char**
			-- each char** implicit allocation through split function

in_arrow_flag	-- int**
				-- allocated fully
				-- (p->pipes_count + 1) int*
				-- allocated count_in_redirection() int's for each int* 

out_arrow_flag	-- int**
				-- allocated fully
				-- (p->pipes_count + 1) int*
				-- allocated count_out_redirection() int's for each int*

output_files	--  char***
				-- allocated partially
				-- (p->pipes_count + 2) char**
				-- allocated count_out_redirections() char*'s for each of
				   the char**'s

input_files_delimeters	--  char***
						-- allocated partially
						-- (p->pipes_count + 2) char**
						-- allocated count_in_redirections() char*'s for 
						   each of the char**'s 

*/