/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 02:46:33 by anasr             #+#    #+#             */
/*   Updated: 2022/03/16 02:47:14 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
********
cmd_part                  --  char *** -- allocated fully     -- (p->pipes_count + 2) char** -- each char** implicit allocation through split function
cmd_path                  --  char **  -- allocated fully     -- (p->pipes_count + 2) char*  -- each char* implicit allocation through get_cmd_path function
cmd                       --  char *** -- allocated partially -- (p->pipes_count + 2) char** -- allocated count_cmds_wout_meta() char*'s for each char**
in_arrow_flag             --  int**    -- allocated fully     -- (p->pipes_count + 1) int*   -- allocated count_in_redirection() int's for each int* 
out_arrow_flag            --  int**    -- allocated fully     -- (p->pipes_count + 1) int*   -- allocated count_out_redirection() int's for each int*
output_files              --  char***  -- allocated partially -- (p->pipes_count + 2) char** -- allocated count_out_redirections() char*'s for each of the char**'s 
input_files_delimeters    --  char***  -- allocated partially -- (p->pipes_count + 2) char** -- allocated count_in_redirections() char*'s for each of the char**'s 
********
*/

void	free_everything(t_parser_info *p)
{
	//free fully
	free_double_int(p->in_arrow_flag, p->pipes_count + 1);
	free_double_int(p->out_arrow_flag, p->pipes_count + 1);
	free_triple_char(p->cmd_part);
	if (p->pipes_count > 0)
		free_double(p->cmd_array);
	//free partially
	free_triple_char_partial(p->cmd);
	free_triple_char_partial(p->output_files);
	free_triple_char_partial(p->input_files_delimiters);
	free_triple_char_partial(p->in_arrow_flag);
	
}

void	allocate_meme_general(t_parser_info *p)
{
	//thats all of the allocation required
	p->cmd_part = (char ***)ft_calloc(p->pipes_count + 2, sizeof(char **));
	p->cmd_path = (char **)ft_calloc(p->pipes_count + 2, sizeof(char *));
	//more allocation will come later
	p->cmd = (char ***)ft_calloc(p->pipes_count + 2, sizeof(char **));
	p->in_arrow_flag = (int **)ft_calloc(p->pipes_count + 1, sizeof(int *));
	p->out_arrow_flag = (int **)ft_calloc(p->pipes_count + 1, sizeof(int *));
	p->output_files = (char ***)ft_calloc(p->pipes_count + 2, sizeof(char **));
	p->input_files_delimiters = (char ***)ft_calloc(p->pipes_count + 2, sizeof(char **));
	
}

void	allocate_meme_specific(char *str, int array_index,t_parser_info *p)
{
	p->cmd[array_index] = (char **)ft_calloc(count_cmds_wout_meta(str), sizeof(char *));
	p->in_arrow_flag[array_index] = (int *)ft_calloc(count_in_redirections(str), sizeof(int));
	p->out_arrow_flag[array_index] = (int *)ft_calloc(count_out_redirections(str), sizeof(int));
	p->output_files[array_index] = (char **)ft_calloc(count_out_redirections(str), sizeof(char *));
	p->input_files_delimiters[array_index] = (char **)ft_calloc(count_in_redirections(str), sizeof(char *));
}
