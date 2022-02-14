/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/02/14 18:07:44 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*input;
	char	*cmd_path;
	char	**split_array;

	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline("\e[34mMINISHELL>\e[0m");
		if (input[0])
			add_history(input);
		split_array = ft_split(input, ' ');
		cmd_path = get_cmd_path(env, split_array[0]);
		printf("%s\n", cmd_path);
		free(cmd_path);
		free(input);
		free_array(split_array);
	}
}