/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Alia <Alia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/02/21 19:33:49 by Alia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_input_output_files_n_cmds(char **words, t_parser_info *p)
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
				p->in_arrow_flag[in_index] = DOUBLE_ARROW;
			else
				p->in_arrow_flag[in_index] = SINGLE_ARROW;
			p->input_files_delimiters[in_index++] = words[++i];
		}
		else if ((!ft_strncmp(words[i], ">>", 2) || !ft_strncmp(words[i], ">", 1)) && words[i + 1])
		{
			if (!ft_strncmp(words[i], ">>", 2))
				p->out_arrow_flag[out_index] = DOUBLE_ARROW;
			else
				p->out_arrow_flag[out_index] = SINGLE_ARROW;
			p->output_files[out_index++] = words[++i];
		}
		else
			p->cmd[cmd_index++] = words[i];
		i++;
	}
}

int	main()
{
	t_parser_info	p;
	char			*input;

	ft_bzero(&p, sizeof(t_parser_info));
	while (1)
	{
		input = readline("\e[35mbaby shell> \e[0m");
		if (input[0])
			add_history(input);
		split_input(input, &p);
		save_input_output_files_n_cmds(p.words, &p);
		p.cmd_path = get_cmd_path(p.cmd[0]);
		execute_command(&p);

		/*-----------------*/
		ft_bzero(p.input_files_delimiters, sizeof(p.input_files_delimiters));
		ft_bzero(p.output_files, sizeof(p.output_files));
		ft_bzero(p.cmd, sizeof(p.cmd));
		free_array(p.words);
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
