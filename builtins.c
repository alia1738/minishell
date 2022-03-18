/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 12:12:03 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/18 17:01:35 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_parser_info *p)
{
	int	i;

	i = 0;
	while (p->env[i])
		printf("%s\n", p->env[i++]);
	p -> exit_code = 0;
	return(0);
}

int	pwd(t_parser_info *p)
{
	char	cwd[PATH_MAX];
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: pwd");
		p->exit_code = 1;
	}
	else
		printf("%s\n", cwd);
	p->exit_code = 0;
	return (0);
}

int	echo(t_parser_info *p, char **argv)
{
	int		i;
	bool	newline_flag;

	newline_flag = true;
	i = 1;
	if (argv[1] && !ft_strncmp(argv[1], "-n", 3))
	{
		newline_flag = false;
		i = 2;
	}
	if (argv[i])
		printf("%s", argv[i++]);
	while (argv[i] && printf(" "))
		printf("%s", argv[i++]);
	if (newline_flag)
		printf("\n");
	p->exit_code = 0;
	return (0);
}

int	export(t_parser_info *p, char **cmd)
{
	int	i;
	
	i = 1;
	while (cmd[i])
		p->env = export_env(p, p->env, cmd[i++]);
	return (0);
}

int	unset(t_parser_info *p, char **cmd)
{
	int	i;
	
	i = 1;
	while (cmd[i])
		p->env = unset_env(p, p->env, cmd[i++]);
	return (0);
}

int	cd(t_parser_info *p, char **argv)
{
	char	*temp;
	char	cwd[PATH_MAX];
	
	p->exit_code = 0;
	if (!argv[1])
	{
		temp = local_getenv("HOME", p->env);
		if (!temp)
		{
			printf("minishell cd: HOME not set\n");
			p->exit_code = 1;
			return (0);
		}
		if (!opendir(temp) || chdir(temp) == -1)
		{
			perror(ft_strjoin("minishell: cd: ", argv[1]));
			p->exit_code = 1;
			return (0);
		}
	}
	else if (access(argv[1], F_OK) != 0 || ft_strlen(argv[1]) > 255)
	{
		if (ft_strlen(argv[1]) > 255)
			printf("minishell: cd: %s: File name too long\n", argv[1]);
		else
			printf("minishell: cd: %s: No such file or directory\n", argv[1]);
		p->exit_code = 1;
		return (0);
	}
	else
	{
		if (!opendir(argv[1]) || chdir(argv[1]) == -1)
		{
			perror(ft_strjoin("minishell: cd: ", argv[1]));
			p->exit_code = 1;
			return (0);
		}
	}
	if (p->exit_code == 0)//if cd succeeds
	{
		export_env(p, p->env, ft_strjoin("OLDPWD=", local_getenv("PWD", p->env)));
		getcwd(cwd, sizeof(cwd));
		export_env(p, p->env, ft_strjoin("PWD=", cwd));
	}
	return(0);
}

void	baby_exit(t_parser_info *p, char **cmd)
{
	//catch cases of failure
	// if (!ft_strncmp(local_getenv("SHLVL"), "1", 2))
	// 	printf("logout\n");
	// else
	printf("exit\n");

	if (!cmd[1])
		p->exit_code = (unsigned char)p->exit_code;
	else
	{
		if (ft_isdigit(cmd[1][0]) == 1)
			p->exit_code = (unsigned char)ft_atoi(cmd[1]);
		else
		{
			p->exit_code = 255;
			printf("minishell: exit: %s: numeric argument required\n", cmd[1]);
		}
	}
	printf("code: %d\n", p->exit_code);
	exit(p->exit_code);
	//if cmd[0] is higer than longmax it should fail
	//also other cases should be accounted for where exit fails
	//too many cases!!!
}