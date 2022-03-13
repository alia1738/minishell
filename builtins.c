/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 12:12:03 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/13 17:39:23 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_parser_info *p)
{
	int	i;

	i = 0;
	while (p->env[i])
		printf("%s\n", p->env[i++]);
	return(0);
}

int	pwd(void)
{
	char	cwd[PATH_MAX];
	if (!getcwd(cwd, sizeof(cwd)))
		perror("minishell: pwd");
	else
		printf("%s\n", cwd);
	return (0);
}

int	cd(char **argv)
{
	if (!argv[1] | !argv[2])
	{	
		if (!argv[1])
		{
			if (!opendir(getenv("HOME")) || chdir(getenv("HOME")) == -1)
				perror(ft_strjoin("minishell: cd: ", argv[1]));
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
		return(0); //return or exit.. we need to consider
	}
	else
		printf("minishell: cd: too many arguments\n");
	exit(1);
}

int	echo(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
		printf("%s ", argv[i++]);
	printf("\n");
	return (0);
}

int	export(t_parser_info *p, char **cmd)
{
	int	i;
	
	i = 1;
	while (cmd[i])
		p->env = export_env(p->env, cmd[i++]);
	return (0);
}
