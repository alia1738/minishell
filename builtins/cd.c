/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 15:22:36 by anasr             #+#    #+#             */
/*   Updated: 2022/03/03 12:54:19 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **env)
{
	(void)env;
	if ((argc == 1) | (argc == 2))
	{	
		if (argc == 1)
		{
			if (!opendir(getenv("HOME")) || chdir(getenv("HOME")) == -1)
				perror(ft_strjoin("minishell: cd: ", argv[1]));
			export_env(environ, ft_strjoin("PWD=", getenv("HOME")));
		}
		else if (access(argv[1], F_OK) != 0 || ft_strlen(argv[1]) > 255)
		{
			if (ft_strlen(argv[1]) > 255)
				printf("minishell: cd: %s: File name too long\n", argv[1]);
			else
				printf("minishell: cd: %s: No such file or directory\n", argv[1]);
			exit(1);
		}
		else
		{
			if (!opendir(argv[1]) || chdir(argv[1]) == -1)
				perror(ft_strjoin("minishell: cd: ", argv[1]));
			printf("PWD: %s\n", getenv("PWD"));
		}
		printf("%s\n", getcwd(NULL, 100));
		export_env(environ, ft_strjoin("PWD=", argv[1]));
		exit(0);
	}
	else
		printf("minishell: cd: too many arguments\n");
	exit(1);
}
