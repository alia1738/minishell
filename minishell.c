/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/03/17 12:59:00 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_input_output_files_n_cmds(int array_index, char **cmd_part, t_parser_info *p)
{
	int	i;
	int	in_index;
	int	out_index;
	int	cmd_index;

	i = 0;
	in_index = 0;
	out_index = 0;
	cmd_index = 0;
	while (cmd_part[i])
	{
		if ((!ft_strncmp(cmd_part[i], "<<", 2) || !ft_strncmp(cmd_part[i], "<", 1)) && cmd_part[i + 1])
		{
			if (!ft_strncmp(cmd_part[i], "<<", 2))
				p->in_arrow_flag[array_index][in_index] = DOUBLE_ARROW;
			else
				p->in_arrow_flag[array_index][in_index] = SINGLE_ARROW;
			p->input_files_delimiters[array_index][in_index++] = cmd_part[++i];
		}
		else if ((!ft_strncmp(cmd_part[i], ">>", 2) || !ft_strncmp(cmd_part[i], ">", 1)) && cmd_part[i + 1])
		{
			if (!ft_strncmp(cmd_part[i], ">>", 2))
				p->out_arrow_flag[array_index][out_index] = DOUBLE_ARROW;
			else
				p->out_arrow_flag[array_index][out_index] = SINGLE_ARROW;
			p->output_files[array_index][out_index++] = cmd_part[++i];
		}
		else
		{
			// if (ft_strchr(cmd_part[i], '$') != NULL && p->do_not_expand[i] == false)
			// 	p->cmd[array_index][cmd_index++] = expand_dollar(cmd_part[i]);
			// else
				p->cmd[array_index][cmd_index++] = cmd_part[i];
		}
		i++;
	}
}

void	save_cmds(char *input, t_parser_info *p)
{
	int		array_index;
	char	*meta[5] = {"<<", "<", ">>", ">", 0};

	p->pipes_count = count_pipes(input);
	allocate_meme_general(p);
	if (p->pipes_count == 0)
	{
		allocate_meme_specific(input, 0, p);
		p->cmd_part[0] = ft_split_custom(input, meta);
		save_input_output_files_n_cmds(0, p->cmd_part[0], p);
		execute_command(p);
	}
	else
	{
		array_index = 0;
		p->cmd_array = ft_split(input, '|');//account for '|' in quotes
		while (array_index < p->pipes_count + 1)
		{
			allocate_meme_specific(p->cmd_array[array_index], array_index, p);
			p->cmd_part[array_index] = ft_split_custom(p->cmd_array[array_index], meta);
			save_input_output_files_n_cmds(array_index, p->cmd_part[array_index], p);
			// p->cmd_path[array_index] = get_cmd_path(p->cmd[array_index][0]);
			array_index++;
		}
		p->cmd_part[array_index] = 0;
		p->cmd[array_index] = 0;
		// printf("HEEYYE\n");
		// init_pipex(p);
		pipe_stuff(p);
		// printf("EXITED PIPEX\n");
		//look into the nulling the end of the p->cmd array **also look into  stack vs heap allocation and the way we should free
	}
	//TESTING
	// int i = -1, j = -1;
	// while (++j < p->pipes_count + 1)
	// {
	// 	i = -1;
	// 	while (p->cmd[j][++i])
	// 		printf("*%s* ", p->cmd[j][i]);
	// 	printf("\n");
	// }
	
	
	/* ----------------------TESTING REDIRECTION-------------------------- */
	// int i, j;	
	// for (j = 0; j < p->pipes_count + 1; j++)
	// {
	// 	printf("COMMAND: %s\n", p->cmd[j][0]);
	// 	for (i = 0; p->input_files_delimiters[j][i]; i++)
	// 	{
	// 		printf("%s Infile/Delimiters: %s ",YELLOW, p->input_files_delimiters[j][i]);
	// 		printf(".. %sInfile Flag: %d ..", BLUE, p->in_arrow_flag[j][i]);
	// 	}
	// 	printf("\n");
	// 	for (i = 0; p->output_files[j][i]; i++)
	// 	{
	// 		printf(" %sInfile/Delimiters: %s ", GREEN, p->output_files[j][i]);
	// 		printf(".. %sInfile Flag: %d ..", MAGENTA, p->out_arrow_flag[j][i]);
	// 	}
	// 	printf("%s\n", RESET);
	// }
}

int	main(int argc, char **argv, char **env)
{
	t_parser_info	p;
	char			*input;

	(void)argc;
	(void)argv;
	ft_bzero(&p, sizeof(t_parser_info));
	p.env = dup_array(env);
	while (1)
	{
		input = readline("\033[1;35mbaby shell\033[2;35m> \e[0m");
		if (input[0])
			add_history(input);
		else
		{
			free(input);
			continue ;
		}
		if (check_repeated_meta(input) == -1)
		{
			printf("minishell: syntax error regarding the usage of metacharacters\n");
			free(input);
			continue ;
		}
		if (!ft_strncmp(input, "exit", 5))
		{
			free(input);
			free_double_char(p.env);
			exit(1);
		}
		save_cmds(input, &p);
		/*-----------------*/
		free_everything(&p);
		free(input);
	}
}

	// printf("count_in_redirection: %d\n➡️", count_in_redirections(input));
	// printf("count_out_redirection: %d\n➡️", count_out_redirections(input));
	// printf("count_wout_meta: %d\n➡️", count_cmds_wout_meta(input));
	
	// int	i = 0;
		// //TESTING - START
		// i = 0;
		// printf("\e[36m****START PRINTING INPUT****\n\e[0m");
		// while (p.cmd_part[i])
		// 	printf("%s\n", p.cmd_part[i++]);
		// printf("\e[36m****END PRINTING INPUT****\n\e[0m");

		// i = 0;
		// printf("\e[32m****START PRINTING COMMANDS****\n\e[0m");
		// printf("{");
		// while (p.cmd[i])
		// 	printf("%s, ", p.cmd[i++]);
		// printf("\b\b}\n");
		// printf("\e[32m****END PRINTING COMMANDS****\n\e[0m");


		// i = 0;
		// printf("****START PRINTING INPUT FILES****\n");
		// while (p.input_files[i])
		// 	printf("%s\n", p.input_files[i++]);
		// printf("****END PRINTING INPUT FILES****\n");

		// i = 0;
		// printf("****START PRINTING OUTPUT FILES****\n");
		// while (p.output_files[i])
		// 	printf("%s\n", p.output_files[i++]);
		// printf("****END PRINTING OUTPUT FILES****\n");
		// //TESTING - END
