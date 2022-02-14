/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/02/14 15:03:44 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char *input;

	(void)argc;
	(void)argv;
	
	while (*envp)
	{
		printf("%s\n", *envp);
		envp++;
	}
	while (1)
	{
		input = readline("\e[34mMINISHELL>\e[0m");
		free(input);
	}
}