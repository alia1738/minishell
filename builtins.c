/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 12:12:03 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/01 17:38:42 by aalsuwai         ###   ########.fr       */
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

int	echo_index(char **argv)
{
	int	index;
	int	array_index;

	array_index = 1;
	while (argv[array_index])
	{
		if (argv[array_index][0] != '-' || argv[array_index][1] != 'n')
			return (array_index);
		index = 2;
		while (argv[array_index][index])
		{
			if (argv[array_index][index] != 'n')
				return (array_index);
			index++;
		}
		array_index++;
	}
	return (array_index);
}

int	echo(t_parser_info *p, char **argv)
{
	int		i;
	bool	newline_flag;

	i = echo_index(argv);
	newline_flag = true;
	if (i > 1)
		newline_flag = false;
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
	int		i;
	bool	failed;
	
	i = 1;
	failed = false;
	while (cmd[i])
	{
		p->env = export_env(p, p->env, cmd[i++]);
		if (p->exit_code == 1)
			failed = true;	
	}
	if (failed)
		p->exit_code = 1;
	return (0);
}

int	unset(t_parser_info *p, char **cmd)
{
	int		i;
	bool	failed;
	
	i = 1;
	failed = false;
	while (cmd[i])
	{
		p->env = unset_env(p, p->env, cmd[i++]);
		if (p->exit_code == 1)
			failed = true;
	}
	if (failed)
		p->exit_code = 1;
	return (0);
}

int	cd(t_parser_info *p, char **argv)
{
	char	*temp;
	char	*save;
	char	cwd[PATH_MAX];
	
	p->exit_code = 0;
	getcwd(cwd, sizeof(cwd));
	save = ft_strdup(cwd);
	ft_bzero(cwd, PATH_MAX);
	if (!argv[1])
	{
		temp = local_getenv("HOME", p->env);
		if (!temp)
		{
			printf("minishell: cd: HOME not set\n");
			p->exit_code = 1;

		}
		else if (chdir(temp) == -1)
		{
			temp = ft_strjoin("minishell: cd: ", temp);
			perror(temp);
			free(temp);
			p->exit_code = 1;
		}
	}
	else
	{
		if (chdir(argv[1]) == -1)
		{
			temp = ft_strjoin("minishell: cd: ", argv[1]);
			perror(temp);
			free(temp);
			p->exit_code = 1;
		}
	}
	if (p->exit_code == 0)//if cd succeeds
	{
		if (!p->oldpwd_dont_update)
		{
			temp = ft_strjoin("OLDPWD=", save);
			p->env = export_env(p, p->env, temp);
			free(temp);
		}
		getcwd(cwd, sizeof(cwd));
		if (local_getenv("PWD", p->env))
		{
			temp = ft_strjoin("PWD=", cwd);
			p->env = export_env(p, p->env, temp);
			free(temp);
		}
	}
	free(save);
	return(0);
}

void	baby_exit(t_parser_info *p, char **cmd)
{
	//catch cases of failure
	// if (!ft_strncmp(local_getenv("SHLVL"), "1", 2))
	// 	printf("logout\n");
	// else
	bool	exit_flag;

	exit_flag = true;
	if (!p->pipes_count)
		printf("exit\n");
	if (!cmd[1])
		p->exit_code = (unsigned char)p->exit_code;
	else
	{
		if (ft_str_isdigit(cmd[1]) == 1 && !cmd[2] && !check_longmax(cmd[1]))
			p->exit_code = (unsigned char)ft_atoi(cmd[1]);
		else if (ft_str_isdigit(cmd[1]) == 1 && cmd[2] && !check_longmax(cmd[1]))
		{
			p->exit_code = 1;
			exit_flag = false;
			printf("minishell: exit: too many arguments\n");
		}
		else
		{
			p->exit_code = 255;
			printf("minishell: exit: %s: numeric argument required\n", cmd[1]);
		}
	}
	// printf("code: %d\n", p->exit_code);
	if (exit_flag && !p->pipes_count)
	{
		free_everything(p);
		free_double_char(p->env);
		free(p->cmd_path);
		exit(p->exit_code);
	}
	//if cmd[0] is higer than longmax it should fail
	//also other cases should be accounted for where exit fails
	//too many cases!!!
	//exit 9223372036854775807 .. -9223372036854775808
}
/*
Exit cases:

> exit 4dns           -   should exit with 255
> exit 42 dns         -   shouldn't exit and the exit code is 1
> exit 42 4234 23     -   shouldn't exit and the exit code is 1
> exit er42 4234 23   -   should exit 255
 

*/