/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 13:56:18 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/02 17:08:31 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_equal(char *env)
{
	int	index;

	index = 0;
	while (env[index] && env[index] != '=')
		index++;
	return (index);
}

bool	append_env_check(char *new_env_var)
{
	if (new_env_var[find_equal(new_env_var) - 1] == '+')
		return (true);
	return (false);
}

static int	get_index(char *env_variable, char **env)
{
	int	index;
	int	array_index;

	array_index = -1;
	while (env[++array_index])
	{
		index = 0;
		while (env[array_index][index] == env_variable[index] && env[array_index][index] != '=')
			index++;
		if (env[array_index][index] == '=' && ((env_variable[index] == '+' && env_variable[index + 1] == '=') || env_variable[index] == '='))
			return (array_index);
	}
	return (-1);
}

int	error_check(t_parser_info *p, char *new_var)
{
	int	index;

	if (!ft_isalpha(new_var[0]) && (new_var[0] != '_'))
	{
		printf("minishell: export: '%s': not valid identifier\n", new_var);
		p->exit_code = 1;
		return (0);
	}
	index = 1;
	while (new_var[index] && new_var[index] != '=')
	{
		if ((!ft_isalnum(new_var[index]) && new_var[index] != '_' && new_var[index] != '+') || (new_var[index] == '+' && new_var[index + 1] != '='))
		{
			printf("minishell: export: '%s': not valid identifier\n", new_var);
			p->exit_code = 1;
			return (0);
		}
		index++;
	}
	return (index);
}

char	**new_env(char **env, char *new_env_var, int save_index)
{
	int		index;
	char	**new_env;
	char	*append_var;

	index = 0;
	if (save_index >= 0)
	{
		if (append_env_check(new_env_var) == true)
			append_var = ft_strdup(env[save_index]);
		free(env[save_index]);
		if (append_env_check(new_env_var) == true)
		{
			env[save_index] = ft_strjoin(append_var, &new_env_var[find_equal(new_env_var) + 1]);
			free(append_var);
		}
		else
			env[save_index] = ft_strdup(new_env_var);
		return (env);
	}
	else if (save_index == -1)
	{
		while (env[index])
			index++;
		new_env = ft_calloc((index + 2), sizeof(char *));
		index = -1;
		while (env[++index])
			new_env[index] = ft_strdup(env[index]);
		new_env[index] = ft_strdup(new_env_var);
		free_double_char(env);
		return (new_env);
	}
	return (0);
}

char	**export_env(t_parser_info *p, char **env, char *new_env_var) // How To Use --> p->env = export_env(p->env, "NAME=value");
{
	int		save_index;
	
	p->exit_code = 0;
	if (!error_check(p, new_env_var))
		return (env);
	if (!ft_strchr(new_env_var, '='))
		return (env);
	save_index = get_index(new_env_var, env);
	env = new_env(env, new_env_var, save_index);
	if (!ft_strncmp(new_env_var, "OLDPWD=", 7))
		p->oldpwd_dont_update = false;
	return (env);
}
