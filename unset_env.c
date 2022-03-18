/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:15:43 by anasr             #+#    #+#             */
/*   Updated: 2022/03/18 19:15:56 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	check_env_unset(char *env_variable, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], env_variable, ft_strlen(env_variable)))
			return (i);
		i++;
	}
	return (-1);
}

char	**remove_var(char **env, char *to_be_removed_var)
{
	int 	i;
	int		new_index;
	char	**new_env;
	int		env_var_index;

	new_index = 0;
	env_var_index = check_env_unset(to_be_removed_var, env);
	printf("var_index = %d\n", env_var_index);
	if (env_var_index >= 0)
	{
		i = 0;
		while (env[i])
			i++;
		new_env = ft_calloc((i), sizeof(char *));//protect
		i = -1;
		while (env[++i])
		{
			if (i != env_var_index)
				new_env[new_index++] = ft_strdup(env[i]);
		}
		free_double_char(env);
		return (new_env);
	}
	else
		return (env);
}

char	**unset_env(t_parser_info *p, char **env, char *to_be_removed_var)
{
	p->exit_code = 0;
	if (!error_check(p, to_be_removed_var))
		return (env);
	if (ft_strchr(to_be_removed_var, '='))
	{
		printf("minishell: export: '%s': not valid identifier\n", to_be_removed_var);
		p->exit_code = 1;
		return (env);
	}
	return (remove_var(env, to_be_removed_var));
}