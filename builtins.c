/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 12:12:03 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/04 15:18:34 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_execute(t_parser_info *p, int i)
{
	if (compare_caseless(p->cmd[i][0], "echo"))
		return (echo(p, p->cmd[i]));
	else if (ft_smartncmp(p->cmd[i][0], "cd", 3))
		return (cd(p, p->cmd[i]));
	else if (compare_caseless(p->cmd[i][0], "pwd"))
		return (pwd(p));
	else if (ft_smartncmp(p->cmd[i][0], "export", 7))
		return (export(p, p->cmd[i]));
	else if (ft_smartncmp(p->cmd[i][0], "unset", 6))
		return (unset(p, p->cmd[i]));
	else if (compare_caseless(p->cmd[i][0], "env"))
		return (env(p));
	else if (ft_smartncmp(p->cmd[i][0], "clear", 6))
		return (clear(p));
	else if (ft_smartncmp(p->cmd[i][0], "exit", 5))
	{
		baby_exit(p, p->cmd[i]);
		return (0);
	}
	return (-1);
}

int	builtin_check(t_parser_info *p, int i)
{
	if (ft_smartncmp(p->cmd[i][0], "cd", 3) || \
	ft_smartncmp(p->cmd[i][0], "export", 7) || \
	ft_smartncmp(p->cmd[i][0], "unset", 6) || \
	ft_smartncmp(p->cmd[i][0], "clear", 6) || \
	ft_smartncmp(p->cmd[i][0], "exit", 5))
		return (0);
	else if (compare_caseless(p->cmd[i][0], "echo") || \
	compare_caseless(p->cmd[i][0], "pwd") || \
	compare_caseless(p->cmd[i][0], "env"))
		return (1);
	return (2);
}

static void	lonely_export(t_parser_info *p)
{
	int	i;
	int	env_index;

	env_index = 0;
	while (p->env[env_index])
	{
		i = 0;
		printf("declare -x ");
		while (p->env[env_index][i] != '=')
			printf("%c", p->env[env_index][i++]);
		printf("=\"");
		while (p->env[env_index][++i])
			printf("%c", p->env[env_index][i]);
		printf("\"\n");
		env_index++;
	}
}

int	export(t_parser_info *p, char **cmd)
{
	int		i;
	bool	failed;

	i = 1;
	failed = false;
	if (!cmd[1])
	{
		lonely_export(p);
		p->exit_code = 0;
		return (0);
	}
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
