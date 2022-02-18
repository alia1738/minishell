/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/02/18 18:45:42 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	count_redirection(char *input, t_parser_info *p)
// {
// 	char	*ptr;

// 	p->in_single_arrow_count = 0;
// 	p->in_double_arrow_count = 0;
// 	p->out_single_arrow_count = 0;
// 	p->out_double_arrow_count = 0;
// 	ptr = ft_strchr(input, '<');
// 	while (ptr != 0)
// 	{
// 		if (*(ptr + 1) == '<')
// 		{
// 			ptr += 2;
// 			if (ft_isalnum(*ptr) == 0 && ft_isspace(*ptr) == 0)
// 			{
// 				printf("minishell: syntax error near unexpected token\n");
// 				p->exit_code = 258;
// 				return ;
// 			}
// 		}
// 		else
// 		{
// 			p->in_single_arrow_count++;
// 			ptr++;
// 		}
// 		ptr = ft_strchr(ptr, '<');
// 	}	
// 	ptr = ft_strnstr(input, "<<", ft_strlen(input));
// 	while (ptr != 0)
// 	{
// 		p->in_double_arrow_count++;
// 		ptr = ft_strnstr(ptr + 1, "<<", ft_strlen(input));
// 	}
// 	ptr = ft_strchr(input, '>');
// 	while (ptr != NULL)
// 	{
// 		if (*(ptr + 1) == '>')
// 		{
// 			ptr += 2;
// 			if (ft_isalnum(*ptr) == 0 && ft_isspace(*ptr) == 0)
// 			{
// 				printf("minishell: syntax error near unexpected token\n");
// 				p->exit_code = 258;
// 				return ;
// 			}
// 		}
// 		else
// 		{
// 			p->out_single_arrow_count++;
// 			ptr++;
// 		}
// 		ptr = ft_strchr(ptr, '>');
// 	}
// 	ptr = ft_strnstr(input, ">>", ft_strlen(input));
// 	while (ptr != 0)
// 	{ 	
// 		p->out_double_arrow_count++;
// 		ptr = ft_strnstr(ptr + 1, ">>", ft_strlen(input));
// 	}
// 	printf("single_in: %d\ndouble_in: %d\nsingle_out: %d\ndouble_out: %d\n", p->in_single_arrow_count,	p->in_double_arrow_count, 	p->out_single_arrow_count, 	p->out_double_arrow_count);
// }

// int	get_redirection_flag(char *input, t_parser_info *p) //for out redirection
// {
// 	char	*single_arrow;
// 	char	*double_arrow;
// 	char	*temp;

// 	single_arrow = ft_strrchr(input, '>');
// 	double_arrow = NULL;
// 	temp = ft_strnstr(input, ">>", ft_strlen(input));
// 	while (temp)
// 	{
// 		double_arrow = temp;
// 		temp = ft_strnstr(input, ">>", ft_strlen(input));
		
// 	}
// 	if (!single_arrow || !double_arrow)
// 	{
// 		if (!single_arrow && !double_arrow)
// 			;
// 		else if (!single_arrow)
// 			p->out_arrow_flag = DOUBLE_ARROW;
// 		else if (!double_arrow)
// 			p->out_arrow_flag = SINGLE_ARROW;
// 	}
// 	else
// 	{
// 		if (single_arrow > double_arrow)
// 			p->out_arrow_flag = SINGLE_ARROW;
// 		else
// 			p->out_arrow_flag = DOUBLE_ARROW;
// 	}
// 	return 0;
// }


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
			p->input_files[in_index++] = words[++i];
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

int	main(int argc, char **argv, char **env)
{
	t_parser_info p;
	char	*input;

	(void)argc;
	(void)argv;
	(void)env;
	ft_bzero(&p, sizeof(t_parser_info));
	while (1)
	{
		input = readline("\e[35mbaby shell> \e[0m");
		if (input[0])
			add_history(input);
		split_input(input, &p);
		save_input_output_files_n_cmds(p.words, &p);
		p.cmd_path = get_cmd_path(env, p.cmd[0]);


		ft_bzero(p.input_files, sizeof(p.input_files));
		ft_bzero(p.output_files, sizeof(p.output_files));
		ft_bzero(p.cmd, 100);
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
