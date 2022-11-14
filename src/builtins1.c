/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 14:40:20 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/05 14:11:08 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_parser_info *p)
{
	int	i;

	i = 0;
	while (p->env[i])
		printf("%s\n", p->env[i++]);
	return (0);
}

static void	lonely_cd(t_parser_info *p)
{
	char	*temp;

	temp = local_getenv("HOME", p->env);
	if (!temp)
	{
		printf("babyshell: cd: HOME not set\n");
		p->exit_code = 1;
	}
	else if (chdir(temp) == -1)
	{
		temp = ft_strjoin("babyshell: cd: ", temp);
		perror(temp);
		free(temp);
		p->exit_code = 1;
	}
}

static void	move_to_new_dir(t_parser_info *p, char **argv)
{
	char	*temp;

	if (chdir(argv[1]) == -1)
	{
		temp = ft_strjoin("babyshell: cd: ", argv[1]);
		perror(temp);
		free(temp);
		p->exit_code = 1;
	}
}

static void	change_pwd(t_parser_info *p, char cwd[PATH_MAX], char *save)
{
	char	*temp;

	if (!p->oldpwd_dont_update)
	{
		temp = ft_strjoin("OLDPWD=", save);
		p->env = export_env(p, p->env, temp);
		free(temp);
	}
	getcwd(cwd, PATH_MAX);
	if (local_getenv("PWD", p->env))
	{
		temp = ft_strjoin("PWD=", cwd);
		p->env = export_env(p, p->env, temp);
		free(temp);
	}
}

int	cd(t_parser_info *p, char **argv)
{
	char	*save;
	char	cwd[PATH_MAX];

	p->exit_code = 0;
	getcwd(cwd, sizeof(cwd));
	save = ft_strdup(cwd);
	ft_bzero(cwd, PATH_MAX);
	if (!argv[1])
		lonely_cd(p);
	else
		move_to_new_dir(p, argv);
	if (p->exit_code == 0)
		change_pwd(p, cwd, save);
	free(save);
	return (0);
}
