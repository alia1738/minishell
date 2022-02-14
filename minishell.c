/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/02/14 20:04:52 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_redirection(char *input, t_parser_info *p)
{
	char	*ptr;

	p->in_single_arrow_count = 0;
	p->in_double_arrow_count = 0;
	p->out_single_arrow_count = 0;
	p->out_double_arrow_count = 0;
	ptr = ft_strchr(input, '<');
	while (ptr != 0)
	{
		if (*(ptr + 1) == '<')
		{
			ptr += 2;
			if (*ptr = '<')
				
		else
			p->in_single_arrow_count++;
		ptr = ft_strchr(ptr + 1, '<');
	}	
	// ptr = ft_strnstr(input, "<<", ft_strlen(input));
	// while (ptr != 0)
	// {
	// 	p->in_double_arrow_count++;
	// 	ptr = ft_strnstr(ptr + 1, "<<", ft_strlen(input));
	// }
	// ptr = ft_strchr(input, '>');
	// while (ptr != NULL)
	// {
	// 	p->out_single_arrow_count++;
	// 	ptr = ft_strchr(ptr + 1, '>');
	// }
	// ptr = ft_strnstr(input, ">>", ft_strlen(input));
	// while (ptr != 0)
	// {
	// 	p->out_double_arrow_count++;
	// 	ptr = ft_strnstr(ptr + 1, ">>", ft_strlen(input));
	// }
	printf("single_in: %d\ndouble_in: %d\nsingle_out: %d\ndouble_out: %d\n", p->in_single_arrow_count,	p->in_double_arrow_count, 	p->out_single_arrow_count, 	p->out_double_arrow_count);
}

int	get_redirection_flag(char *input, t_parser_info *p)
{
	count_redirection(input, p);
	// while (ft_strchr(input, '<') != 0)
	// {
		
	// }
	return 0;
}

int	main(int argc, char **argv, char **env)
{
	t_parser_info p;
	char	*input;
	// char	*temp;

	(void)argc;
	(void)argv;
	(void)env;
	ft_bzero(&p, sizeof(t_parser_info));
	while (1)
	{
		input = readline("\e[34mbaby shell>\e[0m");
		if (input[0])
			add_history(input);
		p.arrow_flag = get_redirection_flag(input, &p);
		// temp = 
		// ft_split(input, ' ');
		// p.cmd_path = get_cmd_path(env, split_array[0]);



		// printf("%s\n", cmd_path);
		// free(cmd_path);
		free(input);
		// free_array(split_array);
	}
}