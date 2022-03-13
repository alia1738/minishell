/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/03/12 10:38:20 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_input_output_files_n_cmds(int array_index, char **words, t_parser_info *p)
{
	int	i;
	int	in_index;
	int	out_index;
	int	cmd_index;

	i = 0;
	in_index = 0;
	out_index = 0;
	cmd_index = 0;
	while (words[i])
	{
		if ((!ft_strncmp(words[i], "<<", 2) || !ft_strncmp(words[i], "<", 1)) && words[i + 1])
		{
			if (!ft_strncmp(words[i], "<<", 2))
				p->in_arrow_flag[array_index][in_index] = DOUBLE_ARROW;
			else
				p->in_arrow_flag[array_index][in_index] = SINGLE_ARROW;
			p->input_files_delimiters[array_index][in_index++] = words[++i];
		}
		else if ((!ft_strncmp(words[i], ">>", 2) || !ft_strncmp(words[i], ">", 1)) && words[i + 1])
		{
			if (!ft_strncmp(words[i], ">>", 2))
				p->out_arrow_flag[array_index][out_index] = DOUBLE_ARROW;
			else
				p->out_arrow_flag[array_index][out_index] = SINGLE_ARROW;
			p->output_files[array_index][out_index++] = words[++i];
		}
		else
		{
			// if (ft_strchr(words[i], '$') != NULL && p->do_not_expand[i] == false)
			// 	p->cmd[array_index][cmd_index++] = expand_dollar(words[i]);
			// else
				p->cmd[array_index][cmd_index++] = words[i];
		}
		i++;
	}
}

int	count_pipes(char *input)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (input[++i])
	{
		if (input[i] == '\'')
		{
			while (input[++i] != '\'' && input[i])
				;
		}
		else if (input[i] == '\"')
		{
			while (input[++i] != '\"' && input[i])
				;
		}
		else if (input[i] == '|')
			count++;
		
	}
	return (count);
}


void	save_cmds(char *input, t_parser_info *p)
{
	int		array_index;
	char	*meta[5] = {"<<", "<", ">>", ">", 0};

	p->pipes_count = count_pipes(input);
	if (p->pipes_count == 0)
	{
		p->words[0] = ft_split_custom(input, meta);
		save_input_output_files_n_cmds(0, p->words[0], p);
		execute_command(p);
	}
	else
	{
		array_index = 0;
		p->cmd_array = ft_split(input, '|');//account for '|' in quotes
		while (array_index < p->pipes_count + 1)
		{
			p->words[array_index] = ft_split_custom(p->cmd_array[array_index], meta);
			save_input_output_files_n_cmds(array_index, p->words[array_index], p);
			p->cmd_path[array_index] = get_cmd_path(p->cmd[array_index][0]);
			array_index++;
		}
		p->words[array_index] = 0;
		p->cmd[array_index][0] = 0;
		init_pipex(p);
		printf("EXITED PIPEX\n");
		//look into the nulling the end of the p->cmd array **also look into  stack vs heap allocation and the way we should free
	}
	//TESTING
	int i = -1, j = -1;
	while (++j < p->pipes_count + 1)
	{
		i = -1;
		while (p->cmd[j][++i])
			printf("*%s* ", p->cmd[j][i]);
		printf("\n");
	}
	//
}

int	main(int argc, char **argv, char **env)
{
	t_parser_info	p;
	char			*input;

	(void)argc;
	(void)argv;
	ft_bzero(&p, sizeof(t_parser_info));
	p.env = env;
	while (1)
	{
		input = readline("\e[35mbaby shell> \e[0m");
		if (input[0])
			add_history(input);
		if (check_repeated_meta(input) == -1)
		{
			printf("minishell: syntax error regarding the usage of metacharacters\n");
			free(input);
			continue ;
		}
		save_cmds(input, &p);

		/*-----------------*/
		ft_bzero(p.input_files_delimiters, sizeof(p.input_files_delimiters));
		ft_bzero(p.output_files, sizeof(p.output_files));
		ft_bzero(p.cmd, sizeof(p.cmd));
		ft_bzero(p.do_not_expand, sizeof(p.do_not_expand));
		free_triple(p.words);
		free(input);
	}
}

	// int	i = 0;
		// //TESTING - START
		// i = 0;
		// printf("\e[36m****START PRINTING INPUT****\n\e[0m");
		// while (p.words[i])
		// 	printf("%s\n", p.words[i++]);
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
